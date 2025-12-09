// overlay-app/src/renderer/components/TextOverlayLayer.tsx
import React from "react";
import { useOverlayStore } from "../state/overlayStore";

export const TextOverlayLayer: React.FC = () => {
  const { textOverlay } = useOverlayStore();

  if (!textOverlay) return null;

  const baseStyle: React.CSSProperties = {
    position: "absolute",
    maxWidth: "30%",
    padding: "12px 16px",
    backgroundColor: "rgba(0,0,0,0.7)",
    color: "#fff",
    fontSize: 14,
    borderRadius: 8,
    pointerEvents: "none",
    whiteSpace: "pre-wrap",
    zIndex: 2000,
  };

  let positionStyle: React.CSSProperties;
  if (textOverlay.position === "bottom") {
    positionStyle = {
      left: "50%",
      bottom: 24,
      transform: "translateX(-50%)",
    };
  } else {
    // right_panel
    positionStyle = {
      top: 80,
      right: 24,
    };
  }

  return (
    <div style={{ ...baseStyle, ...positionStyle }}>
      {textOverlay.content}
    </div>
  );
};

