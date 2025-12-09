# Phase 3: WiFi/WebSocket Setup Guide

## What Was Implemented

✅ WiFi connection handler
✅ WebSocket client to backend
✅ Automatic fallback to Serial if WiFi fails
✅ Event routing (WebSocket when connected, Serial otherwise)

## Configuration

Edit `firmware/include/config.h`:

```cpp
// Set to true to enable WiFi/WebSocket
static const bool ENABLE_WIFI = true;

// Your WiFi credentials
static const char* WIFI_SSID = "YourWiFiName";
static const char* WIFI_PASSWORD = "YourWiFiPassword";

// Your laptop's IP address (where backend runs)
static const char* BACKEND_IP = "192.168.1.100";  // Change this!
static const int BACKEND_PORT = 8765;
```

## Finding Your Laptop's IP Address

### Windows:
```powershell
ipconfig
```
Look for "IPv4 Address" under your active network adapter.

### Mac/Linux:
```bash
ifconfig
# or
ip addr
```

## Testing the Connection

### 1. Start the Backend

```bash
cd core-backend
python -m venv .venv
.venv\Scripts\activate  # Windows
pip install -r requirements.txt
uvicorn app.main:app --host 0.0.0.0 --port 8765
```

You should see:
```
INFO:     Uvicorn running on http://0.0.0.0:8765
```

### 2. Update Firmware Config

Edit `firmware/include/config.h` with your WiFi and laptop IP.

### 3. Build and Upload Firmware

```bash
cd firmware
pio run --target upload
```

### 4. Check Serial Monitor

You should see:
```
[WiFi] Connected!
[WiFi] IP address: 192.168.1.XXX
[WS] WebSocket client initialized
[WS] Connected to: ws://192.168.1.100:8765/glove
```

### 5. Test Events

When you tap Touch1, you should see in the backend console:
```
Glove Event: CommandEvent(name='enter_cursor_mode', ...)
```

## Troubleshooting

### WiFi Won't Connect
- Check SSID and password are correct
- Ensure ESP32 is in range
- Check 2.4GHz WiFi (ESP32 doesn't support 5GHz)

### WebSocket Won't Connect
- Verify backend is running: `curl http://localhost:8765/`
- Check firewall allows port 8765
- Verify BACKEND_IP matches your laptop's IP
- Try pinging the laptop IP from another device

### Events Not Reaching Backend
- Check Serial Monitor for connection status
- Verify backend shows "Glove connected"
- Check backend logs for errors

### Fallback to Serial
If WiFi fails, events will automatically send via Serial. You can use a Serial-to-WebSocket bridge if needed.

## Next Steps

Once WiFi/WebSocket is working:
1. Test all gestures (tap, double tap, long press)
2. Verify cursor movement events
3. Test slide navigation
4. Move to Phase 4: Overlay App

