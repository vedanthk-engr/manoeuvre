// firmware/src/touch.cpp
// ============================================================================
// SIMPLIFIED TOUCH DETECTION - Based on working example approach
// ============================================================================
// Simple edge detection with direct timing measurements
// Much more reliable than complex state machine

#include "touch.h"
#include "config.h"

TouchButton::TouchButton(int pin) : pin(pin) {}

void TouchButton::begin() {
  // Configure with pull-up resistor (TTP223 sensors are active LOW)
  pinMode(pin, INPUT_PULLUP);
  
  // Initialize state
  currentState = digitalRead(pin);
  lastState = currentState;
  pressTime = 0;
  releaseTime = 0;
  tapCount = 0;
  longPressActive = false;
  longPressEmitted = false;
}

TouchEventType TouchButton::update() {
  unsigned long now = millis();
  
  // Read current state (like working example - simple direct read)
  int reading = digitalRead(pin);
  TouchEventType event = TouchEventType::NONE;
  
  // With INPUT_PULLUP: LOW = pressed/touched, HIGH = not pressed
  // Detect press edge (transition from HIGH to LOW)
  if (reading == LOW && lastState == HIGH) {
    // Press detected - record time
    pressTime = now;
    longPressEmitted = false;
    
    // Check if this is a continuation of a tap sequence
    unsigned long timeSinceLastTap = now - lastTapTime;
    if (timeSinceLastTap > DOUBLE_TAP_GAP || tapCount == 0) {
      // Too much time passed or first tap - new sequence
      tapCount = 0;
      firstTapTime = now;
    }
    // If within DOUBLE_TAP_GAP, we're continuing the sequence (tapCount preserved)
  }
  
  // Detect release edge (transition from LOW to HIGH)
  if (reading == HIGH && lastState == LOW) {
    // Release detected - measure duration (like working example)
    releaseTime = now;
    unsigned long pressDuration = releaseTime - pressTime;
    
    if (longPressActive) {
      // Long press ended
      longPressActive = false;
      event = TouchEventType::LONG_PRESS_END;
      tapCount = 0; // Reset tap count
    } else if (pressDuration >= TAP_MIN_DURATION && pressDuration <= TAP_MAX_DURATION) {
      // Valid tap detected (like working example's short tap logic)
      tapCount++;
      lastTapTime = now;
      if (tapCount == 1) {
        firstTapTime = now;
      }
      // Wait to see if more taps come - don't classify yet
    } else {
      // Press too short (< TAP_MIN_DURATION) or too long (> TAP_MAX_DURATION but < LONG_PRESS)
      // Not a valid tap - reset
      tapCount = 0;
    }
  }
  
  // Check for long press while pressed (like working example's gestureMinHoldTime check)
  if (reading == LOW && !longPressActive) {
    unsigned long pressDuration = now - pressTime;
    if (pressDuration >= LONG_PRESS_DURATION) {
      // Long press threshold reached
      longPressActive = true;
      tapCount = 0; // Cancel any tap sequence
      if (!longPressEmitted) {
        event = TouchEventType::LONG_PRESS_START;
        longPressEmitted = true;
      }
    }
  }
  
  // Classify tap sequence after timeout (like working example's approach)
  // Only classify when sensor is released (HIGH) and we have pending taps
  if (reading == HIGH && tapCount > 0) {
    unsigned long timeSinceLastTap = now - lastTapTime;
    if (timeSinceLastTap > DOUBLE_TAP_GAP) {
      // Enough time passed - classify the sequence
      if (event == TouchEventType::NONE) {  // Don't override long press events
        if (tapCount == 1) {
          event = TouchEventType::SINGLE_TAP;
        } else if (tapCount == 2) {
          event = TouchEventType::DOUBLE_TAP;
        } else if (tapCount >= 3) {
          event = TouchEventType::TRIPLE_TAP;
        }
      }
      tapCount = 0; // Reset
    }
  }
  
  // Update last state
  lastState = reading;
  currentState = reading;
  
  return event;
}
