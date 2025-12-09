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

  bool isPressed() const { return lastRawState; }

private:
  int pin;
  bool lastRawState = false;
  unsigned long lastChangeTime = 0;
  unsigned long lastStableStateTime = 0;
  bool stableState = false;

  // tap tracking
  int tapCount = 0;
  unsigned long firstTapTime = 0;
  unsigned long lastTapTime = 0;

  // long press
  bool longPressActive = false;
};

