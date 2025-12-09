# Manoeuvre Core Backend

Python FastAPI service that receives glove events at `ws://<host>:8765/glove`, manages the state machine, dispatches slide navigation via key injection, and pushes overlay render events to `ws://<host>:8765/overlay`.

## Quickstart (Phase 0)

```bash
cd core-backend
python -m venv .venv
.venv\Scripts\activate  # Windows
pip install -r requirements.txt
uvicorn app.main:app --host 0.0.0.0 --port 8765
```

## Structure

- `app/main.py` – FastAPI entrypoint + WebSocket handlers.
- `app/events.py` – Pydantic models for glove and overlay events.
- `app/state_machine.py` – Mode/state tracking.
- `app/config.py` – Ports and cursor bounds.
- `app/slide_context.py` – Placeholder slide context model.
- `app/ai_engine.py` – Placeholder AI helpers.

