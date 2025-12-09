# Manoeuvre Event Contract
**Strict Protocol for Interoperability**

## 1. Event Contract (Glove → Backend)
**Transport**: WebSocket
**URL**: `ws://<LAPTOP_IP>:8765/glove`

### A. Cursor Movement
```json
{
  "type": "cursor_move",
  "dx": 3,
  "dy": -1,
  "timestamp": 1712345678
}
```

### B. Raw Tap Detection
```json
{
  "type": "action",
  "name": "tap1_double",
  "timestamp": 1712345678
}
```
**Valid Names**: `tap1_single`, `tap1_double`, `tap1_long`, `tap2_single`, `tap2_double`, `tap2_triple`, `tap2_long`

### C. High-level Semantic Commands
```json
{
  "type": "command",
  "name": "enter_annotation_mode",
  "timestamp": 1712345680
}
```
**Valid Names**:
- `enter_cursor_mode`
- `slide_next` / `slide_prev`
- `enter_voice_mode` / `voice_start` / `voice_end`
- `toggle_zoom`
- `enter_annotation_mode`
- `enter_highlight_mode`

---

## 2. Backend → Overlay Contract
**Transport**: WebSocket
**URL**: `ws://localhost:8765/overlay`

### A. Cursor Update
```json
{
  "type": "cursor_update",
  "x": 842,
  "y": 313
}
```

### B. Annotation
```json
{
  "type": "annotation_draw",
  "points": [[x1, y1], [x2, y2], ...]
}
```

### C. Highlight
```json
{
  "type": "highlight_block",
  "bbox": [x, y, width, height]
}
```

### D. Zoom
```json
{
  "type": "zoom_set",
  "center": [x, y],
  "scale": 2.0
}
```

### E. AI Text Overlay
```json
{
  "type": "text_overlay",
  "content": "Summary text...",
  "position": "right_panel"
}
```

## 3. Backend Responsibilities (Port 8765)
- **Endpoints**: `/glove` (recv), `/overlay` (broadcast).
- **State Machine**: `IDLE`, `CURSOR_MODE`, `ANNOTATION_MODE`, `HIGHLIGHT_MODE`, `ZOOM_MODE`, `VOICE_MODE`
- **Actions**:
    - `slide_next`/`slide_prev` -> Keyboard Injection.
    - `cursor_move` -> Update state -> Broadcast `cursor_update`.
