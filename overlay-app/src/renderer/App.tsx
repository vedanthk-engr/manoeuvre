import { useEffect, useState } from "react";
import { connectOverlaySocket, OverlayMessage } from "./wsClient";
import CursorLayer from "./canvas/CursorLayer";
import AnnotationLayer from "./canvas/AnnotationLayer";
import HighlightLayer from "./canvas/HighlightLayer";
import TextOverlayLayer from "./canvas/TextOverlayLayer";

type AnnotationStroke = number[][];
type HighlightBlock = number[];

function App() {
  const [cursor, setCursor] = useState({ x: 0, y: 0 });
  const [annotations, setAnnotations] = useState<AnnotationStroke[]>([]);
  const [highlights, setHighlights] = useState<HighlightBlock[]>([]);
  const [textOverlay, setTextOverlay] = useState<string>("");

  useEffect(() => {
    const socket = connectOverlaySocket((msg: OverlayMessage) => {
      if (msg.type === "cursor_update") {
        setCursor({ x: msg.x, y: msg.y });
      } else if (msg.type === "annotation_draw") {
        setAnnotations((prev) => [...prev, msg.points]);
      } else if (msg.type === "highlight_block") {
        setHighlights((prev) => [...prev, msg.bbox]);
      } else if (msg.type === "text_overlay") {
        setTextOverlay(msg.content);
      }
    });
    return () => socket.close();
  }, []);

  return (
    <div className="overlay-root">
      <CursorLayer x={cursor.x} y={cursor.y} />
      <AnnotationLayer strokes={annotations} />
      <HighlightLayer blocks={highlights} />
      <TextOverlayLayer content={textOverlay} />
    </div>
  );
}

export default App;

