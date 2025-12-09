# Phase 3 Backend - Complete ✅

## Structure Created

```
core-backend/
├── app/
│   ├── __init__.py
│   ├── main.py              ✅ FastAPI app, WS endpoints
│   ├── models.py            ✅ Pydantic models for events
│   ├── state_machine.py     ✅ Mode state & transition logic
│   ├── slide_control.py     ✅ Placeholder for keypress logic
│   ├── overlay_bus.py       ✅ Broadcaster for overlay clients
│   └── config.py            (kept for future use)
├── tests/
│   ├── __init__.py
│   └── mock_glove_client.py ✅ Sends fake events to WS
├── requirements.txt         ✅ Updated
└── README.md                ✅ Updated with instructions
```

## What Was Implemented

### ✅ models.py
- `BaseEvent` with type and optional timestamp
- `CursorMoveEvent` with dx, dy (int)
- `ActionEvent` with name (tap1_single, tap1_double, etc.)
- `CommandEvent` with name (enter_cursor_mode, slide_next, etc.)
- `GloveEvent` union type

### ✅ state_machine.py
- `Mode` enum: IDLE, CURSOR, ANNOTATION, HIGHLIGHT, ZOOM, VOICE
- `State` class with:
  - `cursor_x`, `cursor_y` (float, initialized to 960, 540)
  - `zoom_active` (bool)
  - `apply_cursor_move()` - updates cursor and returns cursor_update action
  - `apply_action()` - returns debug_action
  - `apply_command()` - handles mode changes, zoom toggle, slide commands
  - `apply_event()` - main entry point that routes events

### ✅ overlay_bus.py
- `OverlayBus` class managing set of WebSocket clients
- `register()` / `unregister()` for client management
- `broadcast()` sends JSON string to all connected overlay clients

### ✅ slide_control.py
- `handle_slide_action(direction)` - logs slide actions
- Ready for Phase 5 when pyautogui will be added

### ✅ main.py
- FastAPI app with title "Manoeuvre Backend"
- `GET /health` endpoint
- `WebSocket /glove` endpoint:
  - Accepts connections
  - Parses JSON messages
  - Maps to event models (CursorMoveEvent, ActionEvent, CommandEvent)
  - Calls `state.apply_event()`
  - Routes actions: slide → slide_control, others → overlay_bus
- `WebSocket /overlay` endpoint:
  - Accepts overlay client connections
  - Registers with overlay_bus
  - Receives messages (keeps connection alive)

### ✅ mock_glove_client.py
- Connects to `ws://localhost:8765/glove`
- Sends: enter_cursor_mode command
- Sends: 10 cursor_move events
- Sends: enter_annotation_mode command

### ✅ requirements.txt
- fastapi
- uvicorn[standard]
- pydantic
- websockets
- pyautogui

## Testing

1. **Start backend:**
   ```bash
   cd core-backend
   python -m venv .venv
   .venv\Scripts\activate  # Windows
   pip install -r requirements.txt
   uvicorn app.main:app --host 0.0.0.0 --port 8765 --reload
   ```

2. **Run mock client:**
   ```bash
   python tests/mock_glove_client.py
   ```

3. **Expected output:**
   - Backend logs: "Glove connected"
   - Backend logs: Event processing and actions
   - Backend logs: "Glove disconnected"

## Next Steps

- Phase 4: Overlay App (Electron + React)
- Phase 5: Slide Navigation Integration
- Phase 6: Zoom, Highlight & OCR
- Phase 7: Teaching Coach & Timing

