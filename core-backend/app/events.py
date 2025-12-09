from enum import Enum
from typing import Literal, Optional

from pydantic import BaseModel, Field


class EventType(str, Enum):
    CURSOR_MOVE = "cursor_move"
    ACTION = "action"
    COMMAND = "command"


class CursorMoveEvent(BaseModel):
    type: Literal["cursor_move"]
    dx: float
    dy: float
    timestamp: int


class ActionEvent(BaseModel):
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
    timestamp: int


class CommandEvent(BaseModel):
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
    timestamp: int


IncomingEvent = CursorMoveEvent | ActionEvent | CommandEvent


class OverlayEventType(str, Enum):
    CURSOR_UPDATE = "cursor_update"
    ANNOTATION_DRAW = "annotation_draw"
    HIGHLIGHT_BLOCK = "highlight_block"
    ZOOM_SET = "zoom_set"
    TEXT_OVERLAY = "text_overlay"


class OverlayCursorUpdate(BaseModel):
    type: Literal["cursor_update"]
    x: float
    y: float


class OverlayAnnotationDraw(BaseModel):
    type: Literal["annotation_draw"]
    points: list[list[float]]


class OverlayHighlightBlock(BaseModel):
    type: Literal["highlight_block"]
    bbox: list[float]  # [x, y, width, height]


class OverlayZoomSet(BaseModel):
    type: Literal["zoom_set"]
    center: list[float]
    scale: float


class OverlayTextOverlay(BaseModel):
    type: Literal["text_overlay"]
    content: str
    position: Optional[str] = Field(default="right_panel")

