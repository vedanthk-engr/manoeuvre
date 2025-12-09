# Manoeuvre – Intelligent Gesture-Based Teaching Companion

Manoeuvre is a hardware + AI system that lets professors control slides, annotate, highlight, zoom, and generate content using a gesture-controlled glove and an AI-powered desktop app.

## Structure

- `firmware/` – ESP32-based glove firmware. Reads IMU + touch sensors and sends high-level commands over WebSocket/Serial.
- `core-backend/` – Python backend. Receives glove events, manages a state machine, performs OCR + AI calls, controls slide navigation, and sends overlay-rendering commands.
- `overlay-app/` – Electron + React transparent overlay. Renders cursor, annotations, highlights, zoom, and AI-generated text over any slide deck.
- `docs/` – Architecture, protocols, pinouts, and states/modes references.
- `tests/` – Mock clients and sample event payloads.

## Event Contract (Glove → Backend)

Glove sends JSON events like:

```json
{ "type": "cursor_move", "dx": 3, "dy": -1, "timestamp": 1712345678 }
{ "type": "action", "name": "tap1_double", "timestamp": 1712345680 }
{ "type": "command", "name": "slide_next", "timestamp": 1712345685 }
```

Backend uses these to update modes, move cursor, flip slides, trigger voice capture, etc. See `docs/protocol.md` for the full contract.

## Setup sanity checks (Phase 0)

- Firmware: able to flash a simple ESP32 blink sketch.
- Backend: Python venv created, `pip install -r core-backend/requirements.txt` works.
- Overlay: Node/Electron dev server can open a basic window (`npm install` then `npm run dev` inside `overlay-app/`).
