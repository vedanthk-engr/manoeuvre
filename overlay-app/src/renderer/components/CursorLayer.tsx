// overlay-app/src/renderer/components/CursorLayer.tsx
import React from "react";
import { useOverlayStore } from "../state/overlayStore";

export const CursorLayer: React.FC = () => {
  const { cursorX, cursorY, mode } = useOverlayStore();

  // Optionally change color by mode
  const color =
    mode === "ANNOTATION" ? "#00ff88" :
    mode === "HIGHLIGHT" ? "#ffd400" :
    mode === "VOICE" ? "#ff4081" :
    "#00b0ff";

  return (
    <div
      style={{
        position: "absolute",
        left: cursorX - 8,
        top: cursorY - 8,
        width: 16,
        height: 16,
        borderRadius: "50%",
        border: `2px solid ${color}`,
        boxShadow: `0 0 10px ${color}`,
        pointerEvents: "none",
        transition: "transform 0.03s linear",
        zIndex: 1000,
      }}
    />
  );
};

