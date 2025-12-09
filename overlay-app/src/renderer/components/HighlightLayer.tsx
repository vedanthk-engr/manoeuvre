// overlay-app/src/renderer/components/HighlightLayer.tsx
import React from "react";
import { useOverlayStore } from "../state/overlayStore";

export const HighlightLayer: React.FC = () => {
  const { highlights } = useOverlayStore();

  return (
    <>
      {highlights.map((h) => {
        const [x, y, w, hgt] = h.bbox;
        return (
          <div
            key={h.id}
            style={{
              position: "absolute",
              left: x,
              top: y,
              width: w,
              height: hgt,
              backgroundColor: "rgba(255, 212, 0, 0.3)",
              border: "1px solid rgba(255, 212, 0, 0.8)",
              pointerEvents: "none",
              zIndex: 50,
            }}
          />
        );
      })}
    </>
  );
};

