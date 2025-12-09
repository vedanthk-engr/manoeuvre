export type OverlayMessage =
  | { type: "cursor_update"; x: number; y: number }
  | { type: "annotation_draw"; points: number[][] }
  | { type: "highlight_block"; bbox: number[] }
  | { type: "zoom_set"; center: number[]; scale: number }
  | { type: "text_overlay"; content: string; position?: string };

export function connectOverlaySocket(onMessage: (msg: OverlayMessage) => void) {
  const socket = new WebSocket("ws://localhost:8765/overlay");
  socket.onmessage = (event) => {
    try {
      const data = JSON.parse(event.data) as OverlayMessage;
      onMessage(data);
    } catch (err) {
      console.error("Invalid overlay message", err);
    }
  };
  socket.onopen = () => console.log("Overlay socket connected");
  socket.onclose = () => console.warn("Overlay socket closed");
  return socket;
}

