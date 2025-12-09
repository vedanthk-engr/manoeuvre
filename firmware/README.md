# Manoeuvre Firmware

ESP32 glove firmware (Arduino core). Reads IMU + two touch sensors, classifies taps/gestures, and emits JSON events over Serial/WebSocket that match the shared contract in `docs/protocol.md`.

## Layout (Phase 0 placeholder)

- `platformio.ini` – PlatformIO configuration (ESP32 board).
- `src/main.cpp` – Firmware entrypoint (to be expanded in Phase 1–2).

