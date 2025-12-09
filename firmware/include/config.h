// firmware/include/config.h
#pragma once

// PIN DEFINITIONS
static const int PIN_LED    = 2;
static const int PIN_TOUCH1 = 4;
static const int PIN_TOUCH2 = 15;

// I2C PINS (ESP32 default)
static const int PIN_I2C_SDA = 21;
static const int PIN_I2C_SCL = 22;

// TOUCH TIMING (in ms) - EXTREMELY conservative settings for reliable detection
// These values require very deliberate, clear gestures to avoid false triggers
static const unsigned long TOUCH_DEBOUNCE_MS    = 150;   // Debounce time to ignore noise (very high - filters electrical noise)
static const unsigned long TAP_MIN_DURATION     = 100;   // Minimum press time to register tap (must press firmly and hold)
static const unsigned long TAP_MAX_DURATION     = 500;   // Maximum press time to be a tap (longer window for deliberate taps)
static const unsigned long DOUBLE_TAP_GAP       = 800;   // Time between taps for double/triple (very generous - plenty of time)
static const unsigned long TRIPLE_TAP_GAP       = 800;   // Time between taps for triple (same as double)
static const unsigned long LONG_PRESS_DURATION  = 2000;  // > this = long press (2 seconds - must hold very deliberately)

// IMU / CURSOR
static const float CURSOR_GAIN_X = 3.0f;   // tweak for sensitivity
static const float CURSOR_GAIN_Y = 3.0f;
static const float CURSOR_DEADZONE = 0.03f; // ignore small noise

// LOOP
static const unsigned long LOOP_DT_MS = 10; // ~100 Hz loop

// CURSOR MODE AUTO-DEACTIVATE (inactivity timeout, ms)
static const unsigned long CURSOR_INACTIVITY_TIMEOUT = 10000; // 10 seconds

// WiFi / WebSocket Configuration
// Set ENABLE_WIFI to true to use WebSocket, false to use Serial only
static const bool ENABLE_WIFI = false;  // Set to true when ready to connect
static const char* WIFI_SSID = "YOUR_WIFI_SSID";  // Change this
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";  // Change this
static const char* BACKEND_IP = "192.168.1.100";  // Change to your laptop's IP
static const int BACKEND_PORT = 8765;

