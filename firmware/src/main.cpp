// firmware/src/main.cpp
#include <Arduino.h>
#include "config.h"
#include "events.h"
#include "touch.h"
#include "imu.h"

TouchButton touch1(PIN_TOUCH1);
TouchButton touch2(PIN_TOUCH2);
IMUHandler imu;

enum class Mode {
  IDLE,
  CURSOR,
  ANNOTATION,
  HIGHLIGHT,
  ZOOM,
  VOICE
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
  delay(1000); // Give serial time to initialize
  
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  touch1.begin();
  touch2.begin();

  if (!imu.begin()) {
    Serial.println("MPU6050 init failed");
    digitalWrite(PIN_LED, HIGH); // Keep LED on as error indicator
    while (1) delay(100); // Halt on IMU failure
  } else {
    Serial.println("MPU6050 init OK");
    digitalWrite(PIN_LED, LOW);
  }
  
  Serial.println("Manoeuvre Firmware v2.0 - Event Model Ready");
  Serial.println("Waiting for gestures...");
}

void handleTouch1(TouchEventType ev) {
  switch (ev) {
    case TouchEventType::SINGLE_TAP:
      sendAction("tap1_single");
      // Enter cursor mode
      currentMode = Mode::CURSOR;
      cursorModeEnterTime = millis();
      lastCursorActivity = millis();
      sendCommand("enter_cursor_mode");
      break;

    case TouchEventType::DOUBLE_TAP:
      sendAction("tap1_double");
      // Enter voice mode (arm voice command)
      currentMode = Mode::VOICE;
      sendCommand("enter_voice_mode");
      break;

    case TouchEventType::LONG_PRESS_START:
      sendAction("tap1_long");
      touch1LongPressActive = true;
      touch1LongPressStartTime = millis();
      accumulatedGyroX = 0;
      break;

    case TouchEventType::LONG_PRESS_END:
      touch1LongPressActive = false;
      // Determine slide direction based on accumulated movement
      // Positive gyro X = hand moved right = next slide
      // Negative gyro X = hand moved left = prev slide
      if (accumulatedGyroX > 0.3f) {
        sendCommand("slide_next");
      } else if (accumulatedGyroX < -0.3f) {
        sendCommand("slide_prev");
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
      // Enter annotation mode
      currentMode = Mode::ANNOTATION;
      sendCommand("enter_annotation_mode");
      break;

    case TouchEventType::DOUBLE_TAP:
      sendAction("tap2_double");
      // Toggle zoom
      sendCommand("toggle_zoom");
      // Stay in previous mode
      break;

    case TouchEventType::TRIPLE_TAP:
      sendAction("tap2_triple");
      // Enter highlight mode
      currentMode = Mode::HIGHLIGHT;
      sendCommand("enter_highlight_mode");
      break;

    case TouchEventType::LONG_PRESS_START:
      sendAction("tap2_long");
      // In annotation/highlight mode, this starts drawing/highlighting
      // Backend will handle this based on current mode
      break;

    case TouchEventType::LONG_PRESS_END:
      // End drawing/highlighting
      break;

    default:
      break;
  }
}

void loop() {
  unsigned long now = millis();
  if (now - lastLoopTime < LOOP_DT_MS) return;
  lastLoopTime = now;

  // Read touch sensors
  TouchEventType ev1 = touch1.update();
  TouchEventType ev2 = touch2.update();

  if (ev1 != TouchEventType::NONE) {
    handleTouch1(ev1);
  }
  if (ev2 != TouchEventType::NONE) {
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
      Serial.println("Cursor mode auto-deactivated (inactivity)");
    }
  }
}
