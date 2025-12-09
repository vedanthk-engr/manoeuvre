// overlay-app/src/renderer/App.tsx
import React from "react";
import { useOverlayEvents } from "./hooks/useOverlayEvents";
import { CursorLayer } from "./components/CursorLayer";
import { AnnotationLayer } from "./components/AnnotationLayer";
import { HighlightLayer } from "./components/HighlightLayer";
import { TextOverlayLayer } from "./components/TextOverlayLayer";
import { ModeIndicator } from "./components/ModeIndicator";
import { useOverlayStore } from "./state/overlayStore";

export const App: React.FC = () => {
  useOverlayEvents(); // attaches WS and updates store

  const { zoomActive, zoomScale, zoomCenter } = useOverlayStore();

  const transformStyle = zoomActive
    ? {
        transformOrigin: `${zoomCenter[0]}px ${zoomCenter[1]}px`,
        transform: `scale(${zoomScale})`,
      }
    : {};

  return (
    <div
      className="overlay-root"
      style={{
        position: "fixed",
        inset: 0,
        pointerEvents: "none", // overlay does not block clicks (for now)
      }}
    >
      <div
        className="overlay-content"
        style={{
          width: "100%",
          height: "100%",
          position: "relative",
          ...transformStyle,
        }}
      >
        <AnnotationLayer />
        <HighlightLayer />
        <CursorLayer />
      </div>

      <TextOverlayLayer />
      <ModeIndicator />
    </div>
  );
};

export default App;
