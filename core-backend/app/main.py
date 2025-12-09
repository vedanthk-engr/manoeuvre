# core-backend/app/main.py
import json
import logging
from typing import Optional

import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import JSONResponse

from .models import BaseEvent, CursorMoveEvent, ActionEvent, CommandEvent, GloveEvent
from .state_machine import State
from .overlay_bus import OverlayBus
from .slide_control import handle_slide_action

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger("manoeuvre")

app = FastAPI(title="Manoeuvre Backend")

state = State()
overlay_bus = OverlayBus()


@app.get("/health")
async def health():
    return JSONResponse({"status": "ok"})


def parse_event(raw: dict) -> Optional[GloveEvent]:
    """Try to parse incoming dict into the correct event model."""
    t = raw.get("type")
    try:
        if t == "cursor_move":
            return CursorMoveEvent(**raw)
        elif t == "action":
            return ActionEvent(**raw)
        elif t == "command":
            return CommandEvent(**raw)
    except Exception as e:
        logger.warning(f"Failed to parse event: {e}")
        return None
    return None


@app.websocket("/glove")
async def glove_ws(ws: WebSocket):
    await ws.accept()
    logger.info("Glove connected")

    try:
        while True:
            msg = await ws.receive_text()
            logger.debug(f"Raw glove message: {msg}")
            try:
                data = json.loads(msg)
            except json.JSONDecodeError:
                logger.warning(f"Non-JSON from glove: {msg}")
                continue

            ev = parse_event(data)

            if ev is None:
                logger.warning(f"Unrecognized event: {data}")
                continue

            actions = state.apply_event(ev)
            logger.info(f"Event: {ev} -> actions: {actions}")

            # Route actions
            for act in actions:
                if act.get("type") == "slide":
                    handle_slide_action(act["direction"])
                else:
                    # Send to overlay(s)
                    await overlay_bus.broadcast(json.dumps(act))

    except WebSocketDisconnect:
        logger.info("Glove disconnected")


@app.websocket("/overlay")
async def overlay_ws(ws: WebSocket):
    await ws.accept()
    logger.info("Overlay client connected")
    await overlay_bus.register(ws)

    try:
        while True:
            # For now, overlay doesn't need to send anything
            await ws.receive_text()
    except WebSocketDisconnect:
        logger.info("Overlay client disconnected")
        await overlay_bus.unregister(ws)


if __name__ == "__main__":
    uvicorn.run("app.main:app", host="0.0.0.0", port=8765, reload=True)
