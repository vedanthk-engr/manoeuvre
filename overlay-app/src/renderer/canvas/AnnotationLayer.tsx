type Props = { strokes: number[][][] };

export default function AnnotationLayer({ strokes }: Props) {
  return (
    <svg
      style={{
        position: "fixed",
        inset: 0,
        pointerEvents: "none",
      }}
    >
      {strokes.map((stroke, idx) => (
        <polyline
          key={idx}
          points={stroke.map((p) => p.join(",")).join(" ")}
          stroke="yellow"
          strokeWidth={3}
          fill="none"
          strokeLinecap="round"
          strokeLinejoin="round"
        />
      ))}
    </svg>
  );
}

