// firmware/include/config.h
#pragma once

// PIN DEFINITIONS
static const int PIN_LED    = 2;
static const int PIN_TOUCH1 = 4;
static const int PIN_TOUCH2 = 15;

// I2C PINS (ESP32 default)
static const int PIN_I2C_SDA = 21;
static const int PIN_I2C_SCL = 22;

// TOUCH TIMING (in ms) - Very conservative settings for deliberate gestures
static const unsigned long TOUCH_DEBOUNCE_MS    = 100;   // Debounce time to ignore noise (increased)
static const unsigned long TAP_MIN_DURATION     = 80;    // Minimum press time to register tap (increased - must be deliberate)
static const unsigned long TAP_MAX_DURATION     = 400;   // Maximum press time to be a tap (increased - longer taps allowed)
static const unsigned long DOUBLE_TAP_GAP       = 600;   // Time between taps for double/triple (increased - more time to complete)
static const unsigned long TRIPLE_TAP_GAP       = 600;   // Time between taps for triple (increased)
static const unsigned long LONG_PRESS_DURATION  = 1500;  // > this = long press (significantly increased - must hold firmly)

// IMU / CURSOR
static const float CURSOR_GAIN_X = 3.0f;   // tweak for sensitivity
static const float CURSOR_GAIN_Y = 3.0f;
static const float CURSOR_DEADZONE = 0.03f; // ignore small noise

// LOOP
static const unsigned long LOOP_DT_MS = 10; // ~100 Hz loop

// CURSOR MODE AUTO-DEACTIVATE (inactivity timeout, ms)
static const unsigned long CURSOR_INACTIVITY_TIMEOUT = 10000; // 10 seconds

