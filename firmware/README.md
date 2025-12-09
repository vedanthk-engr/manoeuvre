# Manoeuvre Firmware

ESP32 glove firmware (Arduino core). Reads IMU + two touch sensors, classifies taps/gestures, and emits JSON events over Serial/WebSocket that match the shared contract in `docs/protocol.md`.

## Pin map (locked)
- MPU6050: SDA→GPIO21, SCL→GPIO22, VCC→3.3V, GND→GND
- Touch1: OUT→GPIO4, 3.3V, GND
- Touch2: OUT→GPIO15, 3.3V, GND
- LED: GPIO2 via 220Ω → LED+; LED− → GND

## Phase 2 Architecture

The firmware is now modular with clean separation of concerns, organized for PlatformIO:

### File Structure (PlatformIO)
- `include/config.h` – Pin definitions, timing constants, cursor sensitivity
- `include/events.h` / `src/events.cpp` – JSON event builders (cursor_move, action, command)
- `include/touch.h` / `src/touch.cpp` – TouchButton class for tap pattern detection
- `include/imu.h` / `src/imu.cpp` – IMUHandler class for cursor delta computation
- `include/wifi_ws.h` – Placeholder for Phase 3 WebSocket transport
- `src/main.cpp` – Main loop, state machine, event routing
- `platformio.ini` – PlatformIO configuration with library dependencies

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

**Using PlatformIO (Recommended):**

1. Open VS Code with PlatformIO extension installed
2. Open the `manoeuvre/` repo root folder (or navigate to `firmware/` folder)
3. PlatformIO will automatically detect the firmware project
4. Build: Click ✓ icon or `Ctrl+Shift+P` → "PlatformIO: Build"
5. Upload: Click → icon or `Ctrl+Shift+P` → "PlatformIO: Upload"

PlatformIO will automatically:
- Install required libraries (Adafruit MPU6050, WebSockets, ArduinoJson)
- Handle include paths (headers in `include/` are automatically found)
- Compile and upload to ESP32

### Running Serial Monitor (115200 baud)

**IMPORTANT:** The firmware uses `Serial.begin(115200)`, so the monitor MUST run at 115200 baud.

**Method 1: Using PlatformIO UI (VS Code) - If it shows 9600, use Method 2 or 3**
1. Click the plug icon (🔌) in the PlatformIO toolbar at the bottom
2. Or use `Ctrl+Shift+P` → "PlatformIO: Monitor"
3. **If it shows 9600 instead of 115200**, use Method 2 or 3 below

**Method 2: Change Baud Rate While Monitor is Running (Quick Fix)**
1. Open the Serial Monitor using Method 1
2. **While the monitor terminal is focused**, press: `Ctrl + T`, then press `b`
3. Type `115200` and press Enter
4. The header should now show: `--- Terminal on COMx | 115200 8-N-1`

**Method 3: Using Command Line (Most Reliable)**
1. Open a terminal in VS Code (`Ctrl+`` ` or Terminal → New Terminal)
2. Navigate to the firmware directory:
   ```bash
   cd firmware
   ```
3. Run the monitor with explicit baud rate:
   ```bash
   pio device monitor -b 115200 -e esp32dev
   ```
   Or use the helper script (Windows):
   ```bash
   .\monitor.bat
   ```
   Or specify a port:
   ```bash
   .\monitor.bat COM4
   ```

**Method 4: Using Helper Script (Windows)**
1. Open terminal in VS Code
2. Navigate to firmware folder: `cd firmware`
3. Run: `.\monitor.bat` (or `.\monitor.bat COM4` to specify port)

**Verify Correct Baud Rate:**
The terminal header should show:
```
--- Terminal on COMx | 115200 8-N-1
```

**Troubleshooting:**
- If monitor still shows 9600, **restart VS Code** after making config changes
- The `platformio.ini` has `monitor_speed = 115200` set in both `[platformio]` and `[env:esp32dev]` sections
- VS Code PlatformIO extension sometimes caches settings - restart VS Code if needed
- Method 2 (keyboard shortcut) is the fastest way to fix it on-the-fly

**Library Dependencies** (managed by PlatformIO):
- `adafruit/Adafruit MPU6050`
- `adafruit/Adafruit Unified Sensor`
- `links2004/WebSockets`
- `bblanchon/ArduinoJson`

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

