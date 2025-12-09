# Touch Detection Refinement - v3.0

## What Was Changed

### Complete Rewrite of Touch Detection System

The touch detection has been completely rewritten with a proper state machine to eliminate false triggers and improve accuracy.

### Key Improvements

1. **Proper State Machine**
   - Clear states: IDLE → PRESSED → WAITING_TAPS → (back to IDLE or PRESSED for multi-tap)
   - Separate LONG_PRESS state that doesn't interfere with tap detection
   - No race conditions between tap detection and long press

2. **More Conservative Timing**
   - Debounce: 150ms (was 100ms) - filters electrical noise better
   - Min tap duration: 100ms (was 80ms) - must press more deliberately
   - Max tap duration: 500ms (was 400ms) - more forgiving for deliberate taps
   - Double/triple tap gap: 800ms (was 600ms) - plenty of time to complete gesture
   - Long press: 2000ms (was 1500ms) - must hold for 2 full seconds

3. **Better State Tracking**
   - Tracks press start time accurately
   - Validates tap duration before counting
   - Properly resets tap sequences when timeouts occur
   - Long press detection doesn't interfere with tap counting

4. **Enhanced Debug Output**
   - Shows raw sensor readings and pressed state
   - Includes uptime for timing analysis
   - Clear event logging

## How It Works Now

### State Machine Flow

```
IDLE
  ↓ (press detected)
PRESSED
  ↓ (if held > 2000ms)
LONG_PRESS → (release) → IDLE
  ↓ (if released within 100-500ms)
WAITING_TAPS
  ↓ (if new press within 800ms)
PRESSED (continues sequence)
  ↓ (if no press for 800ms)
IDLE (emits SINGLE/DOUBLE/TRIPLE_TAP)
```

### Gesture Requirements

**Single Tap:**
- Press and hold for 100-500ms
- Release
- Wait 800ms (no additional press)
- Result: SINGLE_TAP event

**Double Tap:**
- Press and hold for 100-500ms
- Release
- Within 800ms, press again for 100-500ms
- Release
- Wait 800ms (no additional press)
- Result: DOUBLE_TAP event

**Triple Tap:**
- Same as double, but with 3 taps total
- All taps must be within 800ms of each other
- Result: TRIPLE_TAP event

**Long Press:**
- Press and hold for > 2000ms (2 seconds)
- Emits LONG_PRESS_START when threshold reached
- Release emits LONG_PRESS_END
- Result: LONG_PRESS_START and LONG_PRESS_END events

## Testing

### Expected Behavior

1. **Single Tap (Touch1)**
   - Firm press for ~200ms
   - Release
   - Should see: `SINGLE_TAP` → `enter_cursor_mode`
   - LED blinks once

2. **Double Tap (Touch1)**
   - Two firm presses within 800ms
   - Should see: `DOUBLE_TAP` → `enter_voice_mode`
   - LED blinks twice

3. **Long Press (Touch1)**
   - Hold for 2+ seconds
   - Should see: `LONG_PRESS_START` → (move hand) → `LONG_PRESS_END` → `slide_next` or `slide_prev`
   - LED blinks once

4. **Triple Tap (Touch2)**
   - Three firm presses within 800ms each
   - Should see: `TRIPLE_TAP` → `enter_highlight_mode`
   - LED blinks three times

### Debug Output

Every 3 seconds, you'll see:
```json
{"debug":"touch_status","t1_raw":1,"t1_pressed":0,"t2_raw":1,"t2_pressed":0,"uptime_ms":12345}
```

- `t1_raw`: Raw digital reading (1=not touched, 0=touched)
- `t1_pressed`: Internal pressed state (1=pressed, 0=not pressed)
- Same for `t2_*`

When events occur:
```json
{"debug":"touch1_event","type":"SINGLE_TAP"}
```

## Troubleshooting

### If gestures are still not working:

1. **Check raw readings**
   - Watch the `touch_status` debug output
   - `t1_raw` and `t2_raw` should change from 1 to 0 when you touch
   - If they don't change, check wiring

2. **Check timing**
   - Single tap: Must hold for at least 100ms
   - Double tap: Second tap must come within 800ms
   - Long press: Must hold for 2000ms (2 seconds)

3. **Too sensitive?**
   - Increase `TOUCH_DEBOUNCE_MS` in `config.h`
   - Increase `TAP_MIN_DURATION` in `config.h`

4. **Not sensitive enough?**
   - Decrease `TOUCH_DEBOUNCE_MS` (but not below 50ms)
   - Decrease `TAP_MIN_DURATION` (but not below 50ms)

5. **False triggers?**
   - Check for loose wiring causing electrical noise
   - Increase debounce time
   - Make sure sensors are properly grounded

## Configuration

All timing values are in `firmware/include/config.h`:

```cpp
static const unsigned long TOUCH_DEBOUNCE_MS    = 150;   // Filter noise
static const unsigned long TAP_MIN_DURATION     = 100;   // Min press time
static const unsigned long TAP_MAX_DURATION     = 500;   // Max press time
static const unsigned long DOUBLE_TAP_GAP       = 800;   // Time between taps
static const unsigned long LONG_PRESS_DURATION  = 2000;  // Long press threshold
```

Adjust these values if needed for your specific hardware setup.

