from app.main import app  # type: ignore  # re-export entrypoint

# Convenience entry so `python main.py` works during Phase 0
if __name__ == "__main__":  # pragma: no cover
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=8765)
