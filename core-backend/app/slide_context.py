from dataclasses import dataclass, field
from typing import Any, List, Tuple


@dataclass
class LayoutBlock:
    type: str
    bbox: Tuple[float, float, float, float]
    text: str


@dataclass
class SlideContext:
    slide_id: int | None = None
    ocr_text: str = ""
    layout: List[LayoutBlock] = field(default_factory=list)
    time_limit_minutes: int | None = None
    enter_time: float | None = None
    annotations: list[Any] = field(default_factory=list)
    highlights: list[Any] = field(default_factory=list)

