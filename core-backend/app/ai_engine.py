# Placeholder for AI pipeline (Whisper, LLM, OCR wiring)


def summarize_slide(text: str, bullets: int = 3) -> str:
    return "\n".join([f"{i+1}. {line}" for i, line in enumerate(text.splitlines()[:bullets])])


def generate_questions(text: str, count: int = 2) -> list[str]:
    return [f"Question {i+1}: ... based on slide text" for i in range(count)]

