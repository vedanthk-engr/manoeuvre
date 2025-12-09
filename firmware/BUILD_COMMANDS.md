# Manoeuvre Firmware - Build, Upload & Monitor Commands

## Quick Reference

### Navigate to Firmware Directory
```bash
cd firmware
```

### 1. Build (Compile Only)
```bash
pio run
```
or
```bash
platformio run
```

### 2. Upload to ESP32
```bash
pio run --target upload
```
or
```bash
platformio run --target upload
```

### 3. Build + Upload (One Command)
```bash
pio run --target upload
```

### 4. Serial Monitor (115200 baud)
```bash
pio device monitor -b 115200
```
or using the helper script (Windows):
```bash
.\monitor.bat
```
or specify a port:
```bash
.\monitor.bat COM4
```

### 5. Upload + Monitor (Combined)
```bash
pio run --target upload && pio device monitor -b 115200
```

---

## Method 2: Using VS Code PlatformIO Extension (GUI)

### Build
1. Click the **✓ (checkmark)** icon in the PlatformIO toolbar (bottom of VS Code)
2. Or press `Ctrl+Alt+B`
3. Or use Command Palette: `Ctrl+Shift+P` → "PlatformIO: Build"

### Upload
1. Click the **→ (arrow)** icon in the PlatformIO toolbar
2. Or press `Ctrl+Alt+U`
3. Or use Command Palette: `Ctrl+Shift+P` → "PlatformIO: Upload"

### Serial Monitor
1. Click the **🔌 (plug)** icon in the PlatformIO toolbar
2. Or use Command Palette: `Ctrl+Shift+P` → "PlatformIO: Monitor"
3. **IMPORTANT**: If monitor shows 9600 baud instead of 115200:
   - While monitor terminal is focused, press: `Ctrl+T`, then `b`
   - Type `115200` and press Enter
   - Header should show: `--- Terminal on COMx | 115200 8-N-1`

---

## Method 3: Using Helper Scripts (Windows)

### Build & Upload
```bash
cd firmware
pio run --target upload
```

### Monitor (Using Helper Script)
```bash
cd firmware
.\monitor.bat
```

### Monitor (Specify Port)
```bash
cd firmware
.\monitor.bat COM4
```

---

## Complete Workflow Example

### First Time Setup
```bash
# Navigate to firmware directory
cd firmware

# Build to check for errors
pio run

# If build succeeds, upload
pio run --target upload

# Open serial monitor
pio device monitor -b 115200
```

### Development Loop (After Initial Setup)
```bash
cd firmware

# Upload and immediately open monitor
pio run --target upload && pio device monitor -b 115200
```

---

## Troubleshooting

### Port Not Found
If upload fails with "port not found":
1. Check which COM port your ESP32 is on:
   ```bash
   pio device list
   ```
2. Specify the port explicitly:
   ```bash
   pio run --target upload --upload-port COM4
   ```

### Monitor Shows Garbage/Wrong Baud Rate
- **ALWAYS use 115200 baud** (firmware uses `Serial.begin(115200)`)
- If VS Code monitor shows 9600, use the helper script:
  ```bash
  .\monitor.bat
  ```
- Or manually set baud: `Ctrl+T`, then `b`, then type `115200`

### Build Errors
- Make sure you're in the `firmware/` directory
- Check that PlatformIO is installed: `pio --version`
- Libraries are auto-installed on first build

---

## Expected Serial Output

After upload, you should see:
```
========================================
Manoeuvre Firmware v2.1 - Event Model Ready
========================================

MODE REFERENCE:
  Touch1 Single Tap    → CURSOR mode (air cursor)
  Touch1 Double Tap    → VOICE mode (voice commands)
  Touch1 Long + Move   → Slide navigation (left/right)
  Touch2 Single Tap    → ANNOTATION mode (drawing)
  Touch2 Double Tap    → ZOOM toggle
  Touch2 Triple Tap    → HIGHLIGHT mode

Touch sensitivity: VERY LOW (deliberate gestures required)
Waiting for gestures...
========================================
```


