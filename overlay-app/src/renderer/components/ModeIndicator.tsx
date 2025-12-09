// overlay-app/src/renderer/components/ModeIndicator.tsx
import React from "react";
import { useOverlayStore } from "../state/overlayStore";

export const ModeIndicator: React.FC = () => {
  const { mode, zoomActive } = useOverlayStore();

  return (
    <div
      style={{
        position: "absolute",
        top: 16,
        left: 16,
        padding: "6px 10px",
        backgroundColor: "rgba(0,0,0,0.5)",
        color: "#fff",
        borderRadius: 6,
        fontSize: 12,
        pointerEvents: "none",
        zIndex: 3000,
      }}
    >
      Mode: {mode} {zoomActive ? " (Zoom)" : ""}
    </div>
  );
};

