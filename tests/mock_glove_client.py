import asyncio
import json
import websockets

GLove_URL = "ws://localhost:8765/glove"

sample_events = [
    {"type": "command", "name": "enter_cursor_mode", "timestamp": 1},
    {"type": "cursor_move", "dx": 10, "dy": 5, "timestamp": 2},
    {"type": "cursor_move", "dx": -3, "dy": 2, "timestamp": 3},
    {"type": "command", "name": "slide_next", "timestamp": 4},
]


async def send_events():
    async with websockets.connect(GLove_URL) as ws:
        for event in sample_events:
            await ws.send(json.dumps(event))
            await asyncio.sleep(0.1)


if __name__ == "__main__":
    asyncio.run(send_events())

