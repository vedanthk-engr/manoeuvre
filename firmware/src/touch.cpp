// firmware/src/touch.cpp
#include "touch.h"
#include "config.h"

TouchButton::TouchButton(int pin) : pin(pin) {}

void TouchButton::begin() {
  pinMode(pin, INPUT);
  lastRawState = digitalRead(pin);
  lastChangeTime = millis();
}

TouchEventType TouchButton::update() {
  unsigned long now = millis();
  bool raw = digitalRead(pin);
  TouchEventType event = TouchEventType::NONE;

  // Edge detection
  if (raw != lastRawState) {
    if (raw == HIGH) {
      // Press down
      unsigned long timeSinceLastTap = now - lastTapTime;
      
      // If enough time passed since last tap, reset counter
      if (timeSinceLastTap > DOUBLE_TAP_GAP) {
        tapCount = 0;
      }
      
      if (tapCount == 0) {
        firstTapTime = now;
      }
      tapCount++;
      lastTapTime = now;
    } else {
      // Released
      unsigned long pressDuration = now - lastChangeTime;
      
      // If long press was active → end
      if (longPressActive) {
        longPressActive = false;
        event = TouchEventType::LONG_PRESS_END;
        tapCount = 0; // Reset tap count after long press
        lastRawState = raw;
        lastChangeTime = now;
        return event;
      }
      
      // Check if this was a short tap (not long press)
      if (pressDuration <= TAP_MAX_DURATION) {
        // Wait to see if more taps come
        // Don't classify yet, let timeout logic handle it
      }
    }
    
    lastRawState = raw;
    lastChangeTime = now;
  }

  // Handle long press start (while still pressed)
  if (lastRawState == HIGH && !longPressActive) {
    unsigned long held = now - lastChangeTime;
    if (held > LONG_PRESS_DURATION) {
      longPressActive = true;
      tapCount = 0;  // consume taps
      event = TouchEventType::LONG_PRESS_START;
      return event;
    }
  }

  // Handle taps: after timeout window, decide single/double/triple
  if (!longPressActive && lastRawState == LOW && tapCount > 0) {
    unsigned long sinceLastTap = now - lastTapTime;
    
    // If enough time has passed since last tap, classify
    if (sinceLastTap > DOUBLE_TAP_GAP) {
      if (tapCount == 1) {
        event = TouchEventType::SINGLE_TAP;
      } else if (tapCount == 2) {
        event = TouchEventType::DOUBLE_TAP;
      } else if (tapCount >= 3) {
        event = TouchEventType::TRIPLE_TAP;
      }
      tapCount = 0;
    }
  }

  return event;
}

