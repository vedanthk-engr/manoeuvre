type Props = { x: number; y: number };

const cursorStyle: React.CSSProperties = {
  position: "fixed",
  width: "16px",
  height: "16px",
  borderRadius: "8px",
  background: "rgba(255, 0, 0, 0.8)",
  pointerEvents: "none",
};

export default function CursorLayer({ x, y }: Props) {
  return <div style={{ ...cursorStyle, left: x, top: y }} />;
}

