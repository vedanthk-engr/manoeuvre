# States and Modes

Backend state machine states:
- `IDLE`
- `CURSOR_MODE`
- `ANNOTATION_MODE`
- `HIGHLIGHT_MODE`
- `ZOOM_MODE`
- `VOICE_MODE`

Transitions (per contract):
- `tap1_single`/`enter_cursor_mode` → `CURSOR_MODE`
- `tap2_single`/`enter_annotation_mode` → `ANNOTATION_MODE`
- `tap2_triple`/`enter_highlight_mode` → `HIGHLIGHT_MODE`
- `tap2_double`/`toggle_zoom` → `ZOOM_MODE` toggle behavior
- `tap1_double`/`enter_voice_mode` → `VOICE_MODE`
- `voice_end` → `IDLE` (until new mode entered)

