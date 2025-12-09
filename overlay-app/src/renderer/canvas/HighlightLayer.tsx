type Props = { blocks: number[][] };

export default function HighlightLayer({ blocks }: Props) {
  return (
    <div
      style={{
        position: "fixed",
        inset: 0,
        pointerEvents: "none",
      }}
    >
      {blocks.map((bbox, idx) => {
        const [x, y, width, height] = bbox;
        return (
          <div
            key={idx}
            style={{
              position: "absolute",
              left: x,
              top: y,
              width,
              height,
              background: "rgba(255, 255, 0, 0.3)",
              border: "1px solid rgba(255, 255, 0, 0.5)",
            }}
          />
        );
      })}
    </div>
  );
}

