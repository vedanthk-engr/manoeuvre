# Manoeuvre Firmware Debugging Guide

## What Was Fixed

### 1. Touch Sensor Configuration
- **Problem**: Touch sensors (TTP223) need pull-up resistors configured
- **Fix**: Changed `pinMode(pin, INPUT)` to `pinMode(pin, INPUT_PULLUP)`
- **Logic Fix**: TTP223 sensors output LOW when touched, HIGH when released (with pull-up)

### 2. Enhanced Debugging Output
Added comprehensive debug messages:
- Boot sequence messages
- Touch sensor raw state readings (every 5 seconds)
- Touch event detection debug
- Heartbeat with current mode status
- IMU initialization status

## What You Should See Now

### On Boot (Press RESET button):
```
Manoeuvre firmware booting...
Initializing touch sensors...
Touch1 initial state: 1
Touch2 initial state: 1
Initializing IMU...
MPU6050 init OK
Manoeuvre Firmware v2.0 - Event Model Ready
Waiting for gestures...
Debug: Tap Touch1 to enter cursor mode
```

### Every 2 seconds (Heartbeat):
```
{"debug":"heartbeat","mode":"IDLE"}
```

### Every 5 seconds (Touch sensor raw states):
```
{"debug":"touch_raw","t1":1,"t2":1}
```
- `t1: 1` = Touch1 not touched (HIGH)
- `t1: 0` = Touch1 touched (LOW)
- Same for `t2`

### When You Tap Touch1:
```
{"debug":"touch1_event","type":"SINGLE_TAP"}
{"type":"action","name":"tap1_single","timestamp":...}
{"type":"command","name":"enter_cursor_mode","timestamp":...}
{"debug":"heartbeat","mode":"CURSOR"}
```

### When You Move Hand (in cursor mode):
```
{"type":"cursor_move","dx":3,"dy":-1,"timestamp":...}
{"type":"cursor_move","dx":0,"dy":2,"timestamp":...}
```

## Troubleshooting

### If you see "MPU6050 init failed":
- Check I2C wiring: SDA→GPIO21, SCL→GPIO22
- Check power connections: VCC→3.3V, GND→GND
- LED will stay ON if IMU fails

### If touch sensors show `t1: 1, t2: 1` and never change:
- Check wiring: Touch1 OUT→GPIO4, Touch2 OUT→GPIO15
- Check power: Both sensors need 3.3V and GND
- Try touching the sensors directly (not through glove)

### If you see heartbeat but no touch events:
- Touch sensors might need adjustment
- Try tapping harder or holding longer
- Check the `{"debug":"touch_raw"}` messages to see if sensors detect anything

### If nothing appears at all:
1. Verify Serial Monitor is at 115200 baud
2. Press RESET button on ESP32
3. Check USB cable connection
4. Try uploading firmware again

## Next Steps

1. **Rebuild and upload** the firmware
2. **Open Serial Monitor** at 115200 baud
3. **Press RESET** button on ESP32
4. **Watch for boot messages** - you should see them immediately
5. **Check heartbeat** - should appear every 2 seconds
6. **Test touch sensors** - tap Touch1 and watch for events

