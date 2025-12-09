# Manoeuvre Firmware

ESP32 glove firmware (Arduino core). Reads IMU + two touch sensors, classifies taps/gestures, and emits JSON events over Serial/WebSocket that match the shared contract in `docs/protocol.md`.

## Pin map (locked)
- MPU6050: SDA→GPIO21, SCL→GPIO22, VCC→3.3V, GND→GND
- Touch1: OUT→GPIO4, 3.3V, GND
- Touch2: OUT→GPIO15, 3.3V, GND
- LED: GPIO2 via 220Ω → LED+; LED− → GND

## Phase 2 Architecture

The firmware is now modular with clean separation of concerns:

### File Structure
- `src/config.h` – Pin definitions, timing constants, cursor sensitivity
- `src/events.h` / `events.cpp` – JSON event builders (cursor_move, action, command)
- `src/touch.h` / `touch.cpp` – TouchButton class for tap pattern detection
- `src/imu.h` / `imu.cpp` – IMUHandler class for cursor delta computation
- `src/wifi_ws.h` – Placeholder for Phase 3 WebSocket transport
- `src/main.cpp` – Main loop, state machine, event routing

### Event Model

The firmware emits three types of JSON events (currently via Serial, WebSocket in Phase 3):

1. **Cursor Movement**: `{"type":"cursor_move","dx":3,"dy":-1,"timestamp":...}`
2. **Raw Actions**: `{"type":"action","name":"tap1_single","timestamp":...}`
3. **Commands**: `{"type":"command","name":"enter_cursor_mode","timestamp":...}`

### Gesture Mapping

**Touch Sensor 1 (GPIO4)**:
- Single tap → `enter_cursor_mode` (cursor follows hand movement)
- Double tap → `enter_voice_mode` (arms voice command)
- Long press + hand movement → `slide_next` / `slide_prev` (based on direction)

**Touch Sensor 2 (GPIO15)**:
- Single tap → `enter_annotation_mode`
- Double tap → `toggle_zoom`
- Triple tap → `enter_highlight_mode`
- Long press → starts drawing/highlighting (in annotation/highlight modes)

### Modes

- `IDLE` – No active mode
- `CURSOR` – Air cursor active (auto-deactivates after 10s inactivity)
- `ANNOTATION` – Drawing mode
- `HIGHLIGHT` – Text highlighting mode
- `ZOOM` – Zoom mode (toggled, doesn't change base mode)
- `VOICE` – Voice command mode

### Building & Uploading

Using PlatformIO:
```bash
cd firmware
pio run -t upload
pio device monitor  # Monitor Serial output at 115200 baud
```

Using Arduino IDE:
- Open `src/main.cpp` (or create a new sketch and copy all files)
- Install libraries: Adafruit MPU6050, Adafruit Unified Sensor, Adafruit BusIO
- Select board: ESP32 Dev Module
- Upload and monitor Serial at 115200

### Testing Phase 2

1. Upload firmware
2. Open Serial Monitor (115200 baud)
3. Perform gestures:
   - Tap Touch1 once → should see `tap1_single` action + `enter_cursor_mode` command
   - Move hand → should see `cursor_move` events with dx/dy
   - Tap Touch1 twice → should see `tap1_double` + `enter_voice_mode`
   - Tap Touch2 once → `tap2_single` + `enter_annotation_mode`
   - Tap Touch2 twice → `tap2_double` + `toggle_zoom`
   - Tap Touch2 three times → `tap2_triple` + `enter_highlight_mode`
   - Long press Touch1 + move hand right/left → `slide_next` / `slide_prev`

## Phase 1 Test Sketches

- `tests/test_led_blink/test_led_blink.ino` – LED blink test
- `tests/test_touch/test_touch.ino` – Touch sensor raw reading
- `tests/test_mpu/test_mpu.ino` – IMU raw values
- `tests/test_ws/test_ws.ino` – Basic WiFi/WebSocket test

