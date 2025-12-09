// overlay-app/src/renderer/wsClient.ts
export function createOverlayWebSocket(onMessage: (data: any) => void) {
  const ws = new WebSocket("ws://localhost:8765/overlay");

  ws.onopen = () => {
    console.log("[Overlay WS] connected");
  };

  ws.onmessage = (event) => {
    try {
      const data = JSON.parse(event.data);
      onMessage(data);
    } catch (err) {
      console.error("Invalid JSON from backend", err);
    }
  };

  ws.onclose = () => {
    console.log("[Overlay WS] disconnected");
    // Optionally implement auto-reconnect
  };

  ws.onerror = (err) => {
    console.error("[Overlay WS] error", err);
  };

  return ws;
}
