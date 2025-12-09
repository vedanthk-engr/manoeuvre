# core-backend/app/models.py
from typing import Literal, Optional
from pydantic import BaseModel


class BaseEvent(BaseModel):
    type: str
    timestamp: Optional[int] = None


class CursorMoveEvent(BaseEvent):
    type: Literal["cursor_move"]
    dx: int
    dy: int


class ActionEvent(BaseEvent):
    type: Literal["action"]
    name: Literal[
        "tap1_single",
        "tap1_double",
        "tap1_long",
        "tap2_single",
        "tap2_double",
        "tap2_triple",
        "tap2_long",
    ]


class CommandEvent(BaseEvent):
    type: Literal["command"]
    name: Literal[
        "enter_cursor_mode",
        "slide_next",
        "slide_prev",
        "enter_voice_mode",
        "voice_start",
        "voice_end",
        "toggle_zoom",
        "enter_annotation_mode",
        "enter_highlight_mode",
    ]


GloveEvent = CursorMoveEvent | ActionEvent | CommandEvent

