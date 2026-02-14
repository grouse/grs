#ifndef WINDOW_GENERATED_H
#define WINDOW_GENERATED_H

extern String string_from_enum(GamepadButton button);
extern String string_from_enum(GamepadAxis axis);
extern String string_from_enum(WindowEventType type);
extern String string_from_enum(InputType type);
extern String string_from_enum(KeyCode_ kc);
extern void input_begin_frame();
extern void set_input_map(InputMapId id);
extern void push_input_layer(InputMapId layer);
extern InputMapId get_input_map();
extern bool input_map_active(InputMapId id);
extern bool text_input_enabled();
extern bool get_input_text(InputId id, TextEvent *dst, InputMapId map_id);
extern bool get_input_axis(InputId id, f32 dst[1], InputMapId map_id = INPUT_MAP_ANY);
extern bool get_input_axis2d(InputId id, f32 dst[2], InputMapId map_id = INPUT_MAP_ANY);
extern bool get_input_cursor(InputId id, f32 dst[2], InputMapId map_id = INPUT_MAP_ANY);
extern bool has_input_cursor(InputId id, InputMapId map_id = INPUT_MAP_ANY);
extern bool get_input_cursor_delta(InputId id, f32 dst[2], InputMapId map_id = INPUT_MAP_ANY);
extern bool has_input_cursor_delta(InputId id, InputMapId map_id = INPUT_MAP_ANY);
extern bool get_input_edge(InputId id, InputMapId map_id = INPUT_MAP_ANY);
extern bool get_input_held(InputId id, InputMapId map_id);
extern bool get_input_mouse(MouseButton btn, InputType type = EDGE_DOWN);
extern u32 hash32(const InputDesc & desc, u32 seed = HASH32_SEED);

#endif // WINDOW_GENERATED_H

#ifdef WINDOW_GENERATED_IMPL
#define WINDOW_INTERNAL
#endif

#if defined(WINDOW_INTERNAL) && !defined(WINDOW_INTERNAL_ONCE)
#define WINDOW_INTERNAL_ONCE

static void reset_input_map(InputMapId map_id);
static bool translate_input_event(DynamicArray<WindowEvent> *queue, InputMapId map_id, WindowEvent event);
static bool translate_input_event(DynamicArray<WindowEvent> *queue, WindowEvent event);

#endif
