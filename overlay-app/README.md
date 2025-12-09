# Manoeuvre Overlay App

Electron + React transparent overlay that displays cursor, annotations, highlights, zoom, and AI text overlays on top of any application.

## Phase 4 Setup

### Prerequisites

- Node.js (v18 or higher)
- Backend server running on `ws://localhost:8765/overlay`

### Installation

```bash
cd overlay-app
npm install
```

### Development

1. **Start the backend** (in another terminal):
   ```bash
   cd core-backend
   python -m venv .venv
   .venv\Scripts\activate  # Windows
   pip install -r requirements.txt
   uvicorn app.main:app --host 0.0.0.0 --port 8765 --reload
   ```

2. **Start the overlay app**:
   ```bash
   npm run dev
   ```

   This will:
   - Start Vite dev server on port 5173
   - Launch Electron window with transparent overlay
   - Connect to backend WebSocket automatically

### Testing

1. **With mock glove client** (in another terminal):
   ```bash
   cd core-backend
   python tests/mock_glove_client.py
   ```

2. **Expected behavior**:
   - Cursor should move on screen
   - Mode indicator should change (top-left)
   - Zoom should toggle when zoom_set events are received

### Build

```bash
npm run build
```

## Structure

- `src/main/electron-main.ts` - Electron main process (creates transparent window)
- `src/renderer/App.tsx` - Main React component
- `src/renderer/state/overlayStore.ts` - Zustand store for overlay state
- `src/renderer/hooks/useOverlayEvents.ts` - WebSocket event handler hook
- `src/renderer/components/` - Layer components:
  - `CursorLayer.tsx` - Visual cursor pointer
  - `AnnotationLayer.tsx` - Drawing strokes
  - `HighlightLayer.tsx` - Highlight rectangles
  - `TextOverlayLayer.tsx` - AI text overlays
  - `ModeIndicator.tsx` - Mode HUD (top-left)

## Event Types

The overlay receives these events from the backend:

- `cursor_update` - Move cursor position
- `mode_change` - Change current mode (CURSOR, ANNOTATION, etc.)
- `zoom_set` - Toggle zoom with center and scale
- `annotation_draw` - Add annotation stroke
- `highlight_block` - Add highlight rectangle
- `text_overlay` - Show AI text content
- `clear_annotations` - Clear all annotations
- `clear_highlights` - Clear all highlights
- `clear_text_overlay` - Clear text overlay

## Features

- ✅ Transparent, always-on-top window
- ✅ Fullscreen overlay (doesn't block clicks)
- ✅ WebSocket connection to backend
- ✅ Cursor visualization with mode-based colors
- ✅ Mode indicator HUD
- ✅ Zoom support via CSS transforms
- ✅ Annotation strokes (SVG)
- ✅ Highlight blocks (semi-transparent rectangles)
- ✅ Text overlay (AI summaries/answers)

## Troubleshooting

### Overlay not showing
- Check backend is running: `curl http://localhost:8765/health`
- Check WebSocket connection in browser console (DevTools)
- Verify Electron window is fullscreen and transparent

### Cursor not moving
- Check backend logs for incoming events
- Verify WebSocket connection is established
- Check browser console for errors

### Window not transparent
- Ensure Electron version supports transparency
- Check `transparent: true` in electron-main.ts
- Try restarting the app
