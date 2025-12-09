// overlay-app/src/renderer/components/AnnotationLayer.tsx
import React from "react";
import { useOverlayStore } from "../state/overlayStore";

export const AnnotationLayer: React.FC = () => {
  const { annotations } = useOverlayStore();

  return (
    <svg
      style={{
        position: "absolute",
        inset: 0,
        pointerEvents: "none",
        zIndex: 100,
      }}
    >
      {annotations.map((stroke) => (
        <polyline
          key={stroke.id}
          points={stroke.points.map((p) => `${p[0]},${p[1]}`).join(" ")}
          stroke="#00ff88"
          strokeWidth={3}
          fill="none"
          strokeLinecap="round"
          strokeLinejoin="round"
        />
      ))}
    </svg>
  );
};

