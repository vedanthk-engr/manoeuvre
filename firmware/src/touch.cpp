// firmware/src/touch.cpp
#include "touch.h"
#include "config.h"

TouchButton::TouchButton(int pin) : pin(pin) {}

void TouchButton::begin() {
  // Configure with pull-up resistor (TTP223 sensors are active LOW)
  pinMode(pin, INPUT_PULLUP);
  lastRawState = digitalRead(pin);
  stableState = lastRawState;
  lastChangeTime = millis();
  lastStableStateTime = millis();
}

TouchEventType TouchButton::update() {
  unsigned long now = millis();
  // TTP223 sensors: LOW when touched, HIGH when not touched (with pull-up)
  bool raw = digitalRead(pin);
  TouchEventType event = TouchEventType::NONE;

  // Debouncing: Track raw state changes
  if (raw != lastRawState) {
    lastChangeTime = now;
    lastRawState = raw;
  }

  // Check if state has been stable for debounce period
  bool stateStable = (now - lastChangeTime) > TOUCH_DEBOUNCE_MS;
  
  // Only process when state is stable and different from last stable state
  if (stateStable && raw != stableState) {
    // State change confirmed after debounce
    stableState = raw;
    
    if (raw == LOW) {  // Pressed (touched) - state just became LOW
      // Press down - record when stable press started
      lastStableStateTime = now;
      unsigned long timeSinceLastTap = now - lastTapTime;
      
      // If enough time passed since last tap, reset counter
      if (timeSinceLastTap > DOUBLE_TAP_GAP) {
        tapCount = 0;
      }
      
      if (tapCount == 0) {
        firstTapTime = now;
      }
    } else {  // raw == HIGH (released/not touched) - state just became HIGH
      // Released - calculate how long it was pressed (from stable press start)
      unsigned long pressDuration = now - lastStableStateTime;
      
      // If long press was active → end
      if (longPressActive) {
        longPressActive = false;
        event = TouchEventType::LONG_PRESS_END;
        tapCount = 0; // Reset tap count after long press
        return event;
      }
      
      // Only count as tap if held for minimum duration
      if (pressDuration >= TAP_MIN_DURATION && pressDuration <= TAP_MAX_DURATION) {
        tapCount++;
        lastTapTime = now;
        // Wait to see if more taps come - don't classify yet
      }
    }
  }

  // Handle long press start (while still pressed - LOW state)
  if (stableState == LOW && !longPressActive && stateStable) {
    unsigned long held = now - lastStableStateTime;
    if (held > LONG_PRESS_DURATION) {
      longPressActive = true;
      tapCount = 0;  // consume taps
      event = TouchEventType::LONG_PRESS_START;
      return event;
    }
  }

  // Handle taps: after timeout window, decide single/double/triple
  // Check when sensor is released (HIGH) and we have pending taps
  if (!longPressActive && stableState == HIGH && tapCount > 0) {
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

