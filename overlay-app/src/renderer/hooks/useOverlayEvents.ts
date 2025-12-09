// overlay-app/src/renderer/hooks/useOverlayEvents.ts
import { useEffect } from "react";
import { createOverlayWebSocket } from "../wsClient";
import { useOverlayStore } from "../state/overlayStore";

export function useOverlayEvents() {
  const {
    setCursor,
    setMode,
    setZoom,
    addAnnotationStroke,
    addHighlightBlock,
    setTextOverlay,
    clearAnnotations,
    clearHighlights,
    clearTextOverlay,
  } = useOverlayStore();

  useEffect(() => {
    const ws = createOverlayWebSocket((msg) => {
      switch (msg.type) {
        case "cursor_update":
          setCursor(msg.x, msg.y);
          break;

        case "mode_change":
          setMode(msg.mode);
          break;

        case "zoom_set":
          setZoom(msg.active, msg.center, msg.scale);
          break;

        case "annotation_draw":
          // Convert points array to [number, number][] tuples
          const points: [number, number][] = msg.points.map((p: number[]) => [p[0], p[1]]);
          addAnnotationStroke(points);
          break;

        case "highlight_block":
          // Ensure bbox is [x, y, width, height]
          const bbox: [number, number, number, number] = [
            msg.bbox[0],
            msg.bbox[1],
            msg.bbox[2],
            msg.bbox[3],
          ];
          addHighlightBlock(bbox);
          break;

        case "text_overlay":
          setTextOverlay(msg.content, msg.position || "right_panel");
          break;

        case "clear_annotations":
          clearAnnotations();
          break;

        case "clear_highlights":
          clearHighlights();
          break;

        case "clear_text_overlay":
          clearTextOverlay();
          break;

        default:
          console.log("Unknown overlay message:", msg);
      }
    });

    return () => ws.close();
  }, [setCursor, setMode, setZoom, addAnnotationStroke, addHighlightBlock, setTextOverlay, clearAnnotations, clearHighlights, clearTextOverlay]);
}

