from enum import Enum


class BackendState(str, Enum):
    IDLE = "IDLE"
    CURSOR_MODE = "CURSOR_MODE"
    ANNOTATION_MODE = "ANNOTATION_MODE"
    HIGHLIGHT_MODE = "HIGHLIGHT_MODE"
    ZOOM_MODE = "ZOOM_MODE"
    VOICE_MODE = "VOICE_MODE"


class StateMachine:
    def __init__(self) -> None:
        self.state: BackendState = BackendState.IDLE

    def set_state(self, state: BackendState) -> None:
        self.state = state

    def reset(self) -> None:
        self.state = BackendState.IDLE

