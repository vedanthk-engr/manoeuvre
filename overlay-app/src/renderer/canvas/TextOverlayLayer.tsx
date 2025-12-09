type Props = { content: string };

export default function TextOverlayLayer({ content }: Props) {
  if (!content) return null;
  return (
    <div
      style={{
        position: "fixed",
        right: 20,
        bottom: 20,
        width: 320,
        padding: "12px 16px",
        background: "rgba(0, 0, 0, 0.65)",
        color: "white",
        borderRadius: 8,
        fontSize: 14,
        lineHeight: 1.4,
        pointerEvents: "none",
        whiteSpace: "pre-wrap",
      }}
    >
      {content}
    </div>
  );
}

