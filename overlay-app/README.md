# Manoeuvre Overlay App

Electron + React transparent overlay that connects to `ws://localhost:8765/overlay` and renders cursor, annotations, highlights, zoom, and AI text overlays.

## Structure

- `src/main/electron-main.ts` – Electron entry creating transparent, always-on-top window.
- `src/renderer/wsClient.ts` – WebSocket client for overlay events.
- `src/renderer/App.tsx` – Root React component wiring overlay layers.
- `src/renderer/canvas/*` – Layered rendering components.

## Dev (Phase 0)

```bash
cd overlay-app
npm install
npm run dev
```

This will run Vite and Electron together (see package.json scripts). The app expects the backend to be running on `ws://localhost:8765/overlay`.
