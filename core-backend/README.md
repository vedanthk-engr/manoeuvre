# Manoeuvre Core Backend

Python FastAPI service that receives glove events at `ws://<host>:8765/glove`, manages the state machine, dispatches slide navigation via key injection, and pushes overlay render events to `ws://<host>:8765/overlay`.

## Phase 3 Setup

### 1. Create Virtual Environment

```bash
cd core-backend
python -m venv .venv
```

### 2. Activate Virtual Environment

**Windows:**
```bash
.venv\Scripts\activate
```

**Linux/Mac:**
```bash
source .venv/bin/activate
```

### 3. Install Dependencies

```bash
pip install -r requirements.txt
```

### 4. Run Backend Server

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8765 --reload
```

You should see:
```
INFO:     Uvicorn running on http://0.0.0.0:8765
```

### 5. Test with Mock Glove Client

In a second terminal (with venv activated):

```bash
cd core-backend
python tests/mock_glove_client.py
```

Expected output in server logs:
- Glove connected
- Event: CursorMoveEvent(...) -> actions: [{'type': 'cursor_update', ...}]
- Event: CommandEvent(name='enter_annotation_mode', ...) -> actions: [{'type': 'mode_change', ...}]

## Structure

- `app/main.py` – FastAPI entrypoint + WebSocket handlers (`/glove`, `/overlay`, `/health`)
- `app/models.py` – Pydantic models for glove events (CursorMoveEvent, ActionEvent, CommandEvent)
- `app/state_machine.py` – State class with mode tracking and event processing
- `app/overlay_bus.py` – Broadcast manager for overlay WebSocket clients
- `app/slide_control.py` – Slide navigation handler (logs for now, pyautogui in Phase 5)
- `tests/mock_glove_client.py` – Test client that sends fake events to backend

## API Endpoints

- `GET /health` – Health check endpoint
- `WebSocket /glove` – Receives events from glove/firmware
- `WebSocket /overlay` – Broadcasts actions to overlay clients

## Event Types

The backend accepts three event types from the glove:

1. **cursor_move**: `{"type":"cursor_move","dx":3,"dy":-1,"timestamp":...}`
2. **action**: `{"type":"action","name":"tap1_single","timestamp":...}`
3. **command**: `{"type":"command","name":"enter_cursor_mode","timestamp":...}`

See `app/models.py` for full event definitions.
