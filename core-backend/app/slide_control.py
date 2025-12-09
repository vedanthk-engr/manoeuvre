# core-backend/app/slide_control.py
import logging

logger = logging.getLogger(__name__)


def handle_slide_action(direction: str) -> None:
    """
    direction: 'next' or 'prev'
    In Phase 5, this will call pyautogui.press('right'/'left').
    For now, just log.
    """
    logger.info(f"[SLIDE] {direction}")

