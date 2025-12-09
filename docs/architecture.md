# Manoeuvre Architecture (Phase 0 snapshot)

## Components

- Firmware (ESP32 glove): IMU + two touch sensors + LED, emits JSON events over WebSocket/Serial.
- Core backend (Python): Receives glove events, runs state machine, dispatches slide navigation, AI, OCR, and overlay instructions.
- Overlay app (Electron + React): Transparent, always-on-top renderer that only consumes backend overlay events.

## Data flows

1. Glove → Backend: `ws://<LAPTOP_IP>:8765/glove` sending `cursor_move`, `action:*`, `command:*`.
2. Backend → Overlay: `ws://<HOST>:8765/overlay` sending `cursor_update`, `annotation_draw`, `highlight_block`, `zoom_set`, `text_overlay`.
3. Backend → Slides: key injection via `pyautogui.press("right"|"left")` on `slide_next` / `slide_prev`.

## Modes

- IDLE, CURSOR_MODE, ANNOTATION_MODE, HIGHLIGHT_MODE, ZOOM_MODE, VOICE_MODE.

## Next steps

- Phase 1: hardware bench tests.
- Phase 2: firmware event model.
- Phase 3–4: backend skeleton + overlay visuals.

