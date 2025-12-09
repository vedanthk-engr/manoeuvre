# core-backend/app/state_machine.py
from enum import Enum, auto
from typing import Optional
from .models import GloveEvent, CursorMoveEvent, ActionEvent, CommandEvent


class Mode(Enum):
    IDLE = auto()
    CURSOR = auto()
    ANNOTATION = auto()
    HIGHLIGHT = auto()
    ZOOM = auto()
    VOICE = auto()


class State:
    def __init__(self) -> None:
        self.mode: Mode = Mode.IDLE
        self.cursor_x: float = 960  # assume 1920x1080, center-ish
        self.cursor_y: float = 540
        self.zoom_active: bool = False

    def apply_cursor_move(self, ev: CursorMoveEvent) -> dict:
        # Update internal cursor coordinates (no bounds check yet)
        self.cursor_x += ev.dx
        self.cursor_y += ev.dy
        return {
            "type": "cursor_update",
            "x": int(self.cursor_x),
            "y": int(self.cursor_y),
        }

    def apply_action(self, ev: ActionEvent) -> Optional[dict]:
        # For now, just log / maybe later we can use raw actions too
        return {
            "type": "debug_action",
            "name": ev.name,
        }

    def apply_command(self, ev: CommandEvent) -> Optional[dict]:
        name = ev.name

        if name == "enter_cursor_mode":
            self.mode = Mode.CURSOR
            return {"type": "mode_change", "mode": "CURSOR"}

        if name == "enter_annotation_mode":
            self.mode = Mode.ANNOTATION
            return {"type": "mode_change", "mode": "ANNOTATION"}

        if name == "enter_highlight_mode":
            self.mode = Mode.HIGHLIGHT
            return {"type": "mode_change", "mode": "HIGHLIGHT"}

        if name == "enter_voice_mode":
            self.mode = Mode.VOICE
            return {"type": "mode_change", "mode": "VOICE"}

        if name == "toggle_zoom":
            self.zoom_active = not self.zoom_active
            return {
                "type": "zoom_set",
                "active": self.zoom_active,
                "center": [int(self.cursor_x), int(self.cursor_y)],
                "scale": 2.0 if self.zoom_active else 1.0,
            }

        if name == "slide_next":
            return {"type": "slide", "direction": "next"}

        if name == "slide_prev":
            return {"type": "slide", "direction": "prev"}

        if name in ("voice_start", "voice_end"):
            return {"type": "voice_event", "name": name}

        return None

    def apply_event(self, ev: GloveEvent) -> list[dict]:
        """Apply an incoming glove event and return zero or more overlay/backend actions."""
        actions: list[dict] = []

        if isinstance(ev, CursorMoveEvent) and self.mode == Mode.CURSOR:
            actions.append(self.apply_cursor_move(ev))
        elif isinstance(ev, ActionEvent):
            actions.append(self.apply_action(ev))
        elif isinstance(ev, CommandEvent):
            cmd_action = self.apply_command(ev)
            if cmd_action:
                actions.append(cmd_action)

        return actions
