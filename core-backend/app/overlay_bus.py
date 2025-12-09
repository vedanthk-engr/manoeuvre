# core-backend/app/overlay_bus.py
from typing import Set
from fastapi import WebSocket


class OverlayBus:
    def __init__(self) -> None:
        self.clients: Set[WebSocket] = set()

    async def register(self, ws: WebSocket) -> None:
        self.clients.add(ws)

    async def unregister(self, ws: WebSocket) -> None:
        self.clients.discard(ws)

    async def broadcast(self, message: str) -> None:
        to_remove = []
        for ws in list(self.clients):
            try:
                await ws.send_text(message)
            except Exception:
                to_remove.append(ws)
        for ws in to_remove:
            self.clients.discard(ws)

