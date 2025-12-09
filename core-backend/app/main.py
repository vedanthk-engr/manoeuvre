import json
from typing import Dict, Optional

import pyautogui
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from pydantic import ValidationError

from . import config
from .events import (
    ActionEvent,
    CommandEvent,
    CursorMoveEvent,
    IncomingEvent,
    OverlayCursorUpdate,
)
from .state_machine import BackendState, StateMachine

app = FastAPI()


class ConnectionManager:
    def __init__(self) -> None:
        self.overlay_connection: Optional[WebSocket] = None
        self.glove_connection: Optional[WebSocket] = None

    async def connect_overlay(self, websocket: WebSocket) -> None:
        await websocket.accept()
        self.overlay_connection = websocket
        print("Overlay connected")

    async def connect_glove(self, websocket: WebSocket) -> None:
        await websocket.accept()
        self.glove_connection = websocket
        print("Glove connected")

    def disconnect_overlay(self) -> None:
        self.overlay_connection = None
        print("Overlay disconnected")

    def disconnect_glove(self) -> None:
        self.glove_connection = None
        print("Glove disconnected")

    async def broadcast_to_overlay(self, message: dict) -> None:
        if self.overlay_connection:
            await self.overlay_connection.send_json(message)


manager = ConnectionManager()
state_machine = StateMachine()

cursor_position: Dict[str, float] = {"x": 0.0, "y": 0.0}


def clamp_cursor(x: float, y: float) -> tuple[float, float]:
    x = max(config.CURSOR_MIN_X, min(x, config.CURSOR_MAX_X))
    y = max(config.CURSOR_MIN_Y, min(y, config.CURSOR_MAX_Y))
    return x, y


def handle_command(event: CommandEvent) -> None:
    if event.name == "slide_next":
        pyautogui.press("right")
    elif event.name == "slide_prev":
        pyautogui.press("left")
    elif event.name == "enter_cursor_mode":
        state_machine.set_state(BackendState.CURSOR_MODE)
    elif event.name == "toggle_zoom":
        state_machine.set_state(BackendState.ZOOM_MODE)
    elif event.name == "enter_annotation_mode":
        state_machine.set_state(BackendState.ANNOTATION_MODE)
    elif event.name == "enter_highlight_mode":
        state_machine.set_state(BackendState.HIGHLIGHT_MODE)
    elif event.name == "enter_voice_mode":
        state_machine.set_state(BackendState.VOICE_MODE)
    elif event.name == "voice_end":
        state_machine.reset()


def handle_cursor_move(event: CursorMoveEvent) -> OverlayCursorUpdate:
    cursor_position["x"] += event.dx
    cursor_position["y"] += event.dy
    cursor_position["x"], cursor_position["y"] = clamp_cursor(
        cursor_position["x"], cursor_position["y"]
    )
    return OverlayCursorUpdate(
        type="cursor_update", x=cursor_position["x"], y=cursor_position["y"]
    )


def parse_event(raw: str) -> IncomingEvent:
    data = json.loads(raw)
    if data.get("type") == "cursor_move":
        return CursorMoveEvent.model_validate(data)
    if data.get("type") == "action":
        return ActionEvent.model_validate(data)
    if data.get("type") == "command":
        return CommandEvent.model_validate(data)
    raise ValidationError("Unsupported event type", CursorMoveEvent)


@app.get("/")
def read_root():
    return {"status": "Manoeuvre Core Active", "port": config.PORT}


@app.websocket("/glove")
async def glove_endpoint(websocket: WebSocket):
    await manager.connect_glove(websocket)
    try:
        while True:
            data = await websocket.receive_text()
            try:
                event = parse_event(data)
                print(f"Glove Event: {event}")

                if isinstance(event, CursorMoveEvent):
                    overlay_event = handle_cursor_move(event)
                    await manager.broadcast_to_overlay(overlay_event.model_dump())
                elif isinstance(event, CommandEvent):
                    handle_command(event)
                elif isinstance(event, ActionEvent):
                    # For now we only log raw tap actions
                    pass
            except ValidationError as exc:
                print(f"Invalid event from glove: {exc}")
            except json.JSONDecodeError:
                print("Invalid JSON received from glove")

    except WebSocketDisconnect:
        manager.disconnect_glove()


@app.websocket("/overlay")
async def overlay_endpoint(websocket: WebSocket):
    await manager.connect_overlay(websocket)
    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        manager.disconnect_overlay()

