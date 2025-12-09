// overlay-app/src/renderer/state/overlayStore.ts
import { create } from 'zustand';

export type Mode = "IDLE" | "CURSOR" | "ANNOTATION" | "HIGHLIGHT" | "ZOOM" | "VOICE";

export interface AnnotationStroke {
  id: string;
  points: [number, number][];
}

export interface HighlightBlock {
  id: string;
  bbox: [number, number, number, number]; // [x, y, width, height]
}

export interface OverlayState {
  cursorX: number;
  cursorY: number;
  mode: Mode;
  zoomActive: boolean;
  zoomCenter: [number, number];
  zoomScale: number;
  annotations: AnnotationStroke[];
  highlights: HighlightBlock[];
  textOverlay: { content: string; position: "right_panel" | "bottom" } | null;
  
  // Actions
  setCursor: (x: number, y: number) => void;
  setMode: (mode: Mode) => void;
  setZoom: (active: boolean, center: [number, number], scale: number) => void;
  addAnnotationStroke: (points: [number, number][]) => void;
  addHighlightBlock: (bbox: [number, number, number, number]) => void;
  setTextOverlay: (content: string, position?: "right_panel" | "bottom") => void;
  clearAnnotations: () => void;
  clearHighlights: () => void;
  clearTextOverlay: () => void;
}

export const useOverlayStore = create<OverlayState>((set) => ({
  // Initial state
  cursorX: 960,
  cursorY: 540,
  mode: "IDLE",
  zoomActive: false,
  zoomCenter: [960, 540],
  zoomScale: 1.0,
  annotations: [],
  highlights: [],
  textOverlay: null,

  // Actions
  setCursor: (x, y) => set({ cursorX: x, cursorY: y }),
  
  setMode: (mode) => set({ mode }),
  
  setZoom: (active, center, scale) => 
    set({ zoomActive: active, zoomCenter: center, zoomScale: scale }),
  
  addAnnotationStroke: (points) =>
    set((state) => ({
      annotations: [
        ...state.annotations,
        { id: `annotation-${Date.now()}-${Math.random()}`, points },
      ],
    })),
  
  addHighlightBlock: (bbox) =>
    set((state) => ({
      highlights: [
        ...state.highlights,
        { id: `highlight-${Date.now()}-${Math.random()}`, bbox },
      ],
    })),
  
  setTextOverlay: (content, position = "right_panel") =>
    set({ textOverlay: { content, position } }),
  
  clearAnnotations: () => set({ annotations: [] }),
  
  clearHighlights: () => set({ highlights: [] }),
  
  clearTextOverlay: () => set({ textOverlay: null }),
}));

