// firmware/src/main.cpp
// ============================================================================
// MANOEUVRE FIRMWARE - Main Control Loop
// ============================================================================
// 
// This firmware implements a gesture-based control system for teaching.
// It reads touch sensors and IMU data, classifies gestures, and emits
// high-level events over Serial/WebSocket.
//
// MODE SYSTEM:
// ------------
// The system operates in distinct modes, each with a specific purpose:
//
// 1. IDLE
//    - Default state, waiting for input
//    - No active functions
//
// 2. CURSOR (Touch1 Single Tap)
//    - Air cursor mode: hand movement controls on-screen cursor
//    - Auto-deactivates after 10 seconds of inactivity
//    - Use case: Pointing at slide content, selecting regions
//
// 3. ANNOTATION (Touch2 Single Tap)
//    - Drawing mode: create annotations on slides
//    - Long press Touch2 to start drawing, move hand to draw, release to stop
//    - Use case: Underlining, circling, drawing arrows
//
// 4. HIGHLIGHT (Touch2 Triple Tap)
//    - Highlight mode: highlight text blocks on slides
//    - Long press Touch2 to start highlighting, move over text, release to stop
//    - Use case: Emphasizing important text sections
//
// 5. ZOOM (Touch2 Double Tap - Toggle Action)
//    - Zoom in/out at cursor position
//    - Double tap to zoom in, double tap again to zoom out
//    - Note: This is a toggle action, doesn't change base mode
//
// 6. VOICE (Touch1 Double Tap)
//    - Voice command mode: ready to accept voice input
//    - Long press Touch1 to start recording, release to stop and process
//    - Use case: "Summarize slide", "Generate questions", etc.
//
// SLIDE NAVIGATION:
// -----------------
// Touch1 Long Press + Hand Movement:
//   - Move hand RIGHT → Next slide
//   - Move hand LEFT  → Previous slide
//   - Requires deliberate movement (>0.3 threshold) to avoid accidents
//
// TOUCH SENSITIVITY:
// ------------------
// Very conservative settings to require deliberate gestures:
//   - Debounce: 100ms (filters noise)
//   - Min tap duration: 80ms (must press firmly)
//   - Long press: 1500ms (must hold for 1.5 seconds)
//   - Double/triple tap gap: 600ms (more time to complete gesture)
//
// ============================================================================

#include <Arduino.h>
#include "config.h"
#include "events.h"
#include "touch.h"
#include "imu.h"

TouchButton touch1(PIN_TOUCH1);
TouchButton touch2(PIN_TOUCH2);
IMUHandler imu;

// ============================================================================
// MODE DEFINITIONS - Clear, distinct modes for different functions
// ============================================================================
enum class Mode {
  IDLE,           // No active mode - waiting for input
  CURSOR,         // Air cursor mode - hand movement controls on-screen cursor
  ANNOTATION,     // Drawing mode - long press Touch2 to draw annotations
  HIGHLIGHT,      // Highlight mode - long press Touch2 to highlight text blocks
  ZOOM,           // Zoom mode (toggled, doesn't change base mode)
  VOICE           // Voice command mode - ready to accept voice input
};

Mode currentMode = Mode::IDLE;
unsigned long lastLoopTime = 0;
unsigned long cursorModeEnterTime = 0;
unsigned long lastCursorActivity = 0;

// For slide navigation: track long press + direction
bool touch1LongPressActive = false;
unsigned long touch1LongPressStartTime = 0;
float accumulatedGyroX = 0; // Track hand movement during long press

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Manoeuvre firmware booting...");
  
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Initializing touch sensors...");
  touch1.begin();
  touch2.begin();
  
  // Debug: Read initial touch sensor states
  Serial.print("Touch1 initial state: ");
  Serial.println(digitalRead(PIN_TOUCH1));
  Serial.print("Touch2 initial state: ");
  Serial.println(digitalRead(PIN_TOUCH2));

  Serial.println("Initializing IMU...");
  if (!imu.begin()) {
    Serial.println("ERROR: MPU6050 init failed - check wiring!");
    digitalWrite(PIN_LED, HIGH); // Keep LED on as error indicator
    Serial.println("Firmware halted - fix IMU wiring and reset");
    while (1) {
      delay(1000);
      Serial.println("Waiting for IMU fix...");
    }
  } else {
    Serial.println("MPU6050 init OK");
    digitalWrite(PIN_LED, LOW);
  }
  
  Serial.println("========================================");
  Serial.println("Manoeuvre Firmware v2.1 - Event Model Ready");
  Serial.println("========================================");
  Serial.println("");
  Serial.println("MODE REFERENCE:");
  Serial.println("  Touch1 Single Tap    → CURSOR mode (air cursor)");
  Serial.println("  Touch1 Double Tap    → VOICE mode (voice commands)");
  Serial.println("  Touch1 Long + Move   → Slide navigation (left/right)");
  Serial.println("  Touch2 Single Tap    → ANNOTATION mode (drawing)");
  Serial.println("  Touch2 Double Tap    → ZOOM toggle");
  Serial.println("  Touch2 Triple Tap    → HIGHLIGHT mode");
  Serial.println("");
  Serial.println("Touch sensitivity: VERY LOW (deliberate gestures required)");
  Serial.println("Waiting for gestures...");
  Serial.println("========================================");
}

void blinkLED(int times, int onTime = 100, int offTime = 100) {
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_LED, HIGH);
    delay(onTime);
    digitalWrite(PIN_LED, LOW);
    if (i < times - 1) delay(offTime);
  }
}

void handleTouch1(TouchEventType ev) {
  switch (ev) {
    case TouchEventType::SINGLE_TAP:
      sendAction("tap1_single");
      // ============================================================
      // MODE: CURSOR
      // Purpose: Control on-screen cursor with hand movement
      // How to use: Move hand to move cursor, auto-deactivates after 10s inactivity
      // ============================================================
      currentMode = Mode::CURSOR;
      cursorModeEnterTime = millis();
      lastCursorActivity = millis();
      sendCommand("enter_cursor_mode");
      Serial.println("========================================");
      Serial.println(">>> MODE: CURSOR");
      Serial.println(">>> Function: Air cursor control");
      Serial.println(">>> Usage: Move hand to control cursor");
      Serial.println(">>> Auto-deactivates after 10s inactivity");
      Serial.println("========================================");
      blinkLED(1, 200, 0); // Single longer blink for cursor mode
      break;

    case TouchEventType::DOUBLE_TAP:
      sendAction("tap1_double");
      // ============================================================
      // MODE: VOICE
      // Purpose: Ready to accept voice commands
      // How to use: Long press Touch1 to start recording, release to stop
      // ============================================================
      currentMode = Mode::VOICE;
      sendCommand("enter_voice_mode");
      Serial.println("========================================");
      Serial.println(">>> MODE: VOICE");
      Serial.println(">>> Function: Voice command input");
      Serial.println(">>> Usage: Long press Touch1 to speak, release to process");
      Serial.println(">>> Examples: 'Summarize slide', 'Generate questions'");
      Serial.println("========================================");
      blinkLED(2, 150, 100); // Double blink for voice mode
      break;

    case TouchEventType::LONG_PRESS_START:
      sendAction("tap1_long");
      touch1LongPressActive = true;
      touch1LongPressStartTime = millis();
      accumulatedGyroX = 0;
      Serial.println(">>> Touch1 LONG PRESS STARTED - Move hand left/right for slide navigation");
      break;

    case TouchEventType::LONG_PRESS_END:
      touch1LongPressActive = false;
      // Determine slide direction based on accumulated movement
      // Positive gyro X = hand moved right = next slide
      // Negative gyro X = hand moved left = prev slide
      if (accumulatedGyroX > 0.3f) {
        sendCommand("slide_next");
        Serial.println(">>> SLIDE: NEXT (hand moved right)");
        blinkLED(1, 100, 0);
      } else if (accumulatedGyroX < -0.3f) {
        sendCommand("slide_prev");
        Serial.println(">>> SLIDE: PREVIOUS (hand moved left)");
        blinkLED(1, 100, 0);
      } else {
        Serial.println(">>> SLIDE NAV: Movement too small, ignored");
      }
      // If movement was too small, ignore (avoid accidental slide changes)
      accumulatedGyroX = 0;
      break;

    default:
      break;
  }
}

void handleTouch2(TouchEventType ev) {
  switch (ev) {
    case TouchEventType::SINGLE_TAP:
      sendAction("tap2_single");
      // ============================================================
      // MODE: ANNOTATION
      // Purpose: Draw annotations on slides
      // How to use: Long press Touch2 to start drawing, move hand to draw, release to stop
      // ============================================================
      currentMode = Mode::ANNOTATION;
      sendCommand("enter_annotation_mode");
      Serial.println("========================================");
      Serial.println(">>> MODE: ANNOTATION");
      Serial.println(">>> Function: Draw annotations on slides");
      Serial.println(">>> Usage: Long press Touch2 to draw, move hand, release to stop");
      Serial.println("========================================");
      blinkLED(1, 250, 0); // Single longer blink for annotation
      break;

    case TouchEventType::DOUBLE_TAP:
      sendAction("tap2_double");
      // ============================================================
      // ACTION: ZOOM TOGGLE
      // Purpose: Zoom in/out at cursor position
      // How to use: Double tap to zoom in, double tap again to zoom out
      // Note: This is a toggle action, doesn't change base mode
      // ============================================================
      sendCommand("toggle_zoom");
      Serial.println("========================================");
      Serial.println(">>> ACTION: ZOOM TOGGLE");
      Serial.println(">>> Function: Zoom in/out at cursor position");
      Serial.println(">>> Usage: Double tap to toggle zoom state");
      Serial.println("========================================");
      blinkLED(2, 120, 100); // Double blink for zoom
      // Stay in previous mode
      break;

    case TouchEventType::TRIPLE_TAP:
      sendAction("tap2_triple");
      // ============================================================
      // MODE: HIGHLIGHT
      // Purpose: Highlight text blocks on slides
      // How to use: Long press Touch2 to start highlighting, move over text, release to stop
      // ============================================================
      currentMode = Mode::HIGHLIGHT;
      sendCommand("enter_highlight_mode");
      Serial.println("========================================");
      Serial.println(">>> MODE: HIGHLIGHT");
      Serial.println(">>> Function: Highlight text blocks on slides");
      Serial.println(">>> Usage: Long press Touch2 to highlight, move over text, release to stop");
      Serial.println("========================================");
      blinkLED(3, 100, 80); // Triple blink for highlight mode
      break;

    case TouchEventType::LONG_PRESS_START:
      sendAction("tap2_long");
      // In annotation/highlight mode, this starts drawing/highlighting
      if (currentMode == Mode::ANNOTATION) {
        Serial.println(">>> ANNOTATION: Drawing started - move hand to draw");
      } else if (currentMode == Mode::HIGHLIGHT) {
        Serial.println(">>> HIGHLIGHT: Highlighting started - move over text");
      }
      // Backend will handle this based on current mode
      break;

    case TouchEventType::LONG_PRESS_END:
      // End drawing/highlighting
      if (currentMode == Mode::ANNOTATION) {
        Serial.println(">>> ANNOTATION: Drawing stopped");
      } else if (currentMode == Mode::HIGHLIGHT) {
        Serial.println(">>> HIGHLIGHT: Highlighting stopped");
      }
      break;

    default:
      break;
  }
}

void loop() {
  unsigned long now = millis();
  
  // Heartbeat every ~2 seconds
  static unsigned long lastBeat = 0;
  if (now - lastBeat > 2000) {
    lastBeat = now;
    Serial.print("{\"debug\":\"heartbeat\",\"mode\":\"");
    switch(currentMode) {
      case Mode::IDLE: Serial.print("IDLE"); break;
      case Mode::CURSOR: Serial.print("CURSOR"); break;
      case Mode::ANNOTATION: Serial.print("ANNOTATION"); break;
      case Mode::HIGHLIGHT: Serial.print("HIGHLIGHT"); break;
      case Mode::ZOOM: Serial.print("ZOOM"); break;
      case Mode::VOICE: Serial.print("VOICE"); break;
    }
    Serial.println("\"}");
  }
  
  // Debug: Print raw touch sensor states every 5 seconds
  static unsigned long lastTouchDebug = 0;
  if (now - lastTouchDebug > 5000) {
    lastTouchDebug = now;
    Serial.print("{\"debug\":\"touch_raw\",\"t1\":");
    Serial.print(digitalRead(PIN_TOUCH1));
    Serial.print(",\"t2\":");
    Serial.print(digitalRead(PIN_TOUCH2));
    Serial.println("}");
  }
  
  if (now - lastLoopTime < LOOP_DT_MS) return;
  lastLoopTime = now;

  // Read touch sensors
  TouchEventType ev1 = touch1.update();
  TouchEventType ev2 = touch2.update();

  if (ev1 != TouchEventType::NONE) {
    Serial.print("{\"debug\":\"touch1_event\",\"type\":\"");
    switch(ev1) {
      case TouchEventType::SINGLE_TAP: Serial.print("SINGLE_TAP"); break;
      case TouchEventType::DOUBLE_TAP: Serial.print("DOUBLE_TAP"); break;
      case TouchEventType::TRIPLE_TAP: Serial.print("TRIPLE_TAP"); break;
      case TouchEventType::LONG_PRESS_START: Serial.print("LONG_PRESS_START"); break;
      case TouchEventType::LONG_PRESS_END: Serial.print("LONG_PRESS_END"); break;
      default: Serial.print("UNKNOWN"); break;
    }
    Serial.println("\"}");
    handleTouch1(ev1);
  }
  if (ev2 != TouchEventType::NONE) {
    Serial.print("{\"debug\":\"touch2_event\",\"type\":\"");
    switch(ev2) {
      case TouchEventType::SINGLE_TAP: Serial.print("SINGLE_TAP"); break;
      case TouchEventType::DOUBLE_TAP: Serial.print("DOUBLE_TAP"); break;
      case TouchEventType::TRIPLE_TAP: Serial.print("TRIPLE_TAP"); break;
      case TouchEventType::LONG_PRESS_START: Serial.print("LONG_PRESS_START"); break;
      case TouchEventType::LONG_PRESS_END: Serial.print("LONG_PRESS_END"); break;
      default: Serial.print("UNKNOWN"); break;
    }
    Serial.println("\"}");
    handleTouch2(ev2);
  }

  // Track hand movement during touch1 long press (for slide navigation)
  if (touch1LongPressActive) {
    float gyroX = imu.getGyroX();
    unsigned long dt = now - touch1LongPressStartTime;
    accumulatedGyroX += gyroX * dt / 1000.0f;
    touch1LongPressStartTime = now;
  }

  // In cursor mode → send cursor deltas
  if (currentMode == Mode::CURSOR) {
    CursorDelta d = imu.computeCursorDelta();
    if (d.dx != 0 || d.dy != 0) {
      sendCursorMove(d.dx, d.dy);
      lastCursorActivity = now;
    }
    
    // Auto-deactivate cursor mode after inactivity timeout
    if (now - lastCursorActivity > CURSOR_INACTIVITY_TIMEOUT) {
      currentMode = Mode::IDLE;
      Serial.println("========================================");
      Serial.println(">>> MODE: IDLE");
      Serial.println(">>> Cursor mode auto-deactivated (10s inactivity)");
      Serial.println("========================================");
      blinkLED(2, 80, 80); // Quick double blink to indicate mode change
    }
  }
}
