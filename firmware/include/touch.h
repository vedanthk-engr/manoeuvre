// firmware/include/touch.h
#pragma once
#include <Arduino.h>

enum class TouchEventType {
  NONE,
  SINGLE_TAP,
  DOUBLE_TAP,
  TRIPLE_TAP,
  LONG_PRESS_START,
  LONG_PRESS_END
};

class TouchButton {
public:
  TouchButton(int pin);

  void begin();
  // call in loop, returns high-level events
  TouchEventType update();

  bool isPressed() const { return currentState == LOW; }  // LOW = pressed with INPUT_PULLUP

private:
  int pin;
  
  // Simple state tracking (like working example)
  int lastState = LOW;  // Last stable state
  int currentState = LOW; // Current reading
  
  // Press/release timing (like working example)
  unsigned long pressTime = 0;
  unsigned long releaseTime = 0;
  
  // Tap sequence tracking
  int tapCount = 0;
  unsigned long lastTapTime = 0;
  unsigned long firstTapTime = 0;
  
  // Long press tracking
  bool longPressActive = false;
  bool longPressEmitted = false;
};
