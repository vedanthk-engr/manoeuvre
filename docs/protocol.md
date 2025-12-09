# Manoeuvre Protocol (Glove ↔ Backend ↔ Overlay)

## Glove → Backend (WebSocket `/glove`)

All messages are JSON lines. No additional types beyond this list.

### Cursor movement
```json
{ "type": "cursor_move", "dx": 3, "dy": -1, "timestamp": 1712345678 }
```

### Raw tap actions
```json
{ "type": "action", "name": "tap1_double", "timestamp": 1712345678 }
```
Valid `name`: `tap1_single`, `tap1_double`, `tap1_long`, `tap2_single`, `tap2_double`, `tap2_triple`, `tap2_long`.

### High-level commands
```json
{ "type": "command", "name": "enter_annotation_mode", "timestamp": 1712345680 }
```
Valid `name`:
- `enter_cursor_mode`
- `slide_next`
- `slide_prev`
- `enter_voice_mode`
- `voice_start`
- `voice_end`
- `toggle_zoom`
- `enter_annotation_mode`
- `enter_highlight_mode`

## Backend → Overlay (WebSocket `/overlay`)

Messages the overlay must consume:

- Cursor update
```json
{ "type": "cursor_update", "x": 842, "y": 313 }
```
- Annotation stroke
```json
{ "type": "annotation_draw", "points": [[10, 20], [12, 24]] }
```
- Highlight block
```json
{ "type": "highlight_block", "bbox": [x, y, width, height] }
```
- Zoom state
```json
{ "type": "zoom_set", "center": [x, y], "scale": 2.0 }
```
- AI text overlay
```json
{ "type": "text_overlay", "content": "Summary text...", "position": "right_panel" }
```

## Slide navigation decision

- Backend handles slide navigation via key injection (`pyautogui.press("right"|"left")`).
- Firmware will not send HID keypresses in v1.

