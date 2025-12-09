# core-backend/tests/mock_glove_client.py
import asyncio
import json
import websockets


async def mock_glove():
    uri = "ws://localhost:8765/glove"
    async with websockets.connect(uri) as ws:
        # enter cursor mode
        await ws.send(json.dumps({
            "type": "command",
            "name": "enter_cursor_mode",
            "timestamp": 1
        }))

        # move cursor a bit
        for i in range(10):
            await ws.send(json.dumps({
                "type": "cursor_move",
                "dx": 5,
                "dy": 3,
                "timestamp": 2 + i
            }))
            await asyncio.sleep(0.1)

        # switch to annotation
        await ws.send(json.dumps({
            "type": "command",
            "name": "enter_annotation_mode",
            "timestamp": 100
        }))


if __name__ == "__main__":
    asyncio.run(mock_glove())

