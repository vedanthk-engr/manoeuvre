# Manoeuvre Firmware

ESP32 glove firmware (Arduino core). Reads IMU + two touch sensors, classifies taps/gestures, and emits JSON events over Serial/WebSocket that match the shared contract in `docs/protocol.md`.

## Pin map (locked)
- MPU6050: SDA→GPIO21, SCL→GPIO22, VCC→3.3V, GND→GND
- Touch1: OUT→GPIO4, 3.3V, GND
- Touch2: OUT→GPIO15, 3.3V, GND
- LED: GPIO2 via 220Ω → LED+; LED− → GND

## Layout
- `platformio.ini` – PlatformIO configuration.
- `src/main.cpp` – Firmware entrypoint (Phase 2+).
- `tests/` – Phase 1 hardware validation sketches:
  - `test_led_blink/test_led_blink.ino`
  - `test_touch/test_touch.ino`
  - `test_mpu/test_mpu.ino`
  - `test_ws/test_ws.ino`

## Running tests (Phase 1)
Use Arduino IDE or PlatformIO:
- LED: open `tests/test_led_blink/test_led_blink.ino`, build & upload → LED on GPIO2 should blink.
- Touch: `tests/test_touch/test_touch.ino` → watch Serial (115200) for T1/T2 = 0/1 when tapping.
- IMU: `tests/test_mpu/test_mpu.ino` (requires Adafruit_MPU6050 library) → see accel values change.
- WiFi/WS: `tests/test_ws/test_ws.ino` (set SSID/password + host IP) → sends simple WS text to backend `/glove`.

