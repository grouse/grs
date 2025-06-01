#include "window.h"

#include "array.h"
#include "map.h"
#include "string.h"
#include "hash.h"

#include "generated/internal/window.h"

struct InputCursor {
    f32 x, y;
    f32 dx, dy;
};

struct InputMap {
    String name;

    DynamicArray<InputDesc> by_device[IDEVICE_MAX][ITYPE_MAX];
    DynamicArray<InputDesc> by_type[ITYPE_MAX][IDEVICE_MAX];

    DynamicMap<InputId, i32> edges;
    DynamicMap<InputId, bool> held;
    DynamicMap<InputId, f32[2]> axes;
    DynamicMap<InputId, InputCursor> cursors;
    DynamicMap<InputDesc, bool> active;
    DynamicMap<InputId, DynamicArray<TextEvent>> text;
};

struct {
    DynamicMap<InputType, u8> mouse;

    DynamicArray<InputMap> maps;
    DynamicArray<InputMapId> layers;
    DynamicArray<InputMapId> queued_layers;
    InputMapId active_map = -1;
} input{};

String string_from_enum(GamepadButton button) EXPORT
{
    switch (button) {
    case PAD_F_UP:    return "PAD_F_UP";
    case PAD_F_DOWN:  return "PAD_F_DOWN";
    case PAD_F_LEFT:  return "PAD_F_LEFT";
    case PAD_F_RIGHT: return "PAD_F_RIGHT";
    case PAD_D_UP:    return "PAD_D_UP";
    case PAD_D_DOWN:  return "PAD_D_DOWN";
    case PAD_D_LEFT:  return "PAD_D_LEFT";
    case PAD_D_RIGHT: return "PAD_D_RIGHT";
    case PAD_LB:      return "PAD_LB";
    case PAD_LS:      return "PAD_LS";
    case PAD_RB:      return "PAD_RB";
    case PAD_RS:      return "PAD_RS";
    }

    return "unknown";
}

String string_from_enum(GamepadAxis axis) EXPORT
{
    switch (axis) {
    case PAD_TRIG0: return "PAD_TRIG0";
    case PAD_TRIG1: return "PAD_TRIG1";
    case PAD_JOY0: return "PAD_JOY0";
    case PAD_JOY1: return "PAD_JOY1";
    }

    return "unknown";
}

String string_from_enum(WindowEventType type) EXPORT
{
    if (type >= WE_INPUT) return "WE_INPUT";

    switch (type) {
    case WE_INVALID: PANIC_UNREACHABLE();
    case WE_INPUT:          return "WE_INPUT";
    case WE_MOUSE_WHEEL:    return "WE_MOUSE_WHEEL";
    case WE_MOUSE_PRESS:    return "WE_MOUSE_PRESS";
    case WE_MOUSE_RELEASE:  return "WE_MOUSE_RELEASE";
    case WE_MOUSE_MOVE:     return "WE_MOUSE_MOVE";
    case WE_KEY_PRESS:      return "WE_KEY_PRESS";
    case WE_KEY_RELEASE:    return "WE_KEY_RELEASE";
    case WE_PAD_CONNECT:    return "WE_PAD_CONNECT";
    case WE_PAD_DISCONNECT: return "WE_PAD_DISCONNECT";
    case WE_PAD_PRESS:      return "WE_PAD_PRESS";
    case WE_PAD_RELEASE:    return "WE_PAD_RELEASE";
    case WE_PAD_AXIS:       return "WE_PAD_AXIS";
    case WE_PAD_AXIS2:      return "WE_PAD_AXIS2";
    case WE_TEXT:           return "WE_TEXT";
    case WE_RESIZE:         return "WE_RESIZE";
    case WE_QUIT:           return "WE_QUIT";
    }

    return "unknown";
}

String string_from_enum(InputType type) EXPORT
{
    switch (type) {
    case EDGE_DOWN:  return "EDGE_DOWN";
    case EDGE_UP:    return "EDGE_UP";
    case HOLD:       return "HOLD";
    case AXIS:       return "AXIS";
    case AXIS_2D:    return "AXIS_2D";
    case CURSOR:     return "CURSOR";
    case TEXT:       return "TEXT";
    case CHORD:      return "CHORD";
    case ITYPE_INVALID: return "INVALID";
    case ITYPE_MAX:     PANIC_UNREACHABLE();
    }

    return "unknown";
}

String string_from_enum(KeyCode_ kc) EXPORT
{
    switch (kc) {
    case KC_LSUPER:    return "KC_LSUPER";
    case KC_RSUPER:    return "KC_RSUPER";
    case KC_APP:       return "KC_APP";
    case KC_SCLK:      return "KC_SCLK";
    case KC_BREAK:     return "KC_BREAK";
    case KC_UNKNOWN:   return "KC_UNKNOWN";
    case KC_BACKSPACE: return "KC_BACKSPACE";
    case KC_ENTER:     return "KC_ENTER";
    case KC_TAB:       return "KC_TAB";
    case KC_INSERT:    return "KC_INSERT";
    case KC_DELETE:    return "KC_DELETE";
    case KC_HOME:      return "KC_HOME";
    case KC_END:       return "KC_END";
    case KC_PAGE_UP:   return "KC_PAGE_UP";
    case KC_PAGE_DOWN: return "KC_PAGE_DOWN";
    case KC_SPACE:     return "KC_SPACE";
    case KC_SHIFT:     return "KC_SHIFT";
    case KC_CTRL:      return "KC_CTRL";
    case KC_ALT:       return "KC_ALT";
    case KC_LBRACKET:  return "KC_LBRACKET";
    case KC_RBRACKET:  return "KC_RBRACKET";
    case KC_BACKSLASH: return "KC_BACKSLASH";
    case KC_COLON:     return "KC_COLON";
    case KC_TICK:      return "KC_TICK";
    case KC_COMMA:     return "KC_COMMA";
    case KC_PERIOD:    return "KC_PERIOD";
    case KC_SLASH:     return "KC_SLASH";
    case KC_1:         return "KC_1";
    case KC_2:         return "KC_2";
    case KC_3:         return "KC_3";
    case KC_4:         return "KC_4";
    case KC_5:         return "KC_5";
    case KC_6:         return "KC_6";
    case KC_7:         return "KC_7";
    case KC_8:         return "KC_8";
    case KC_9:         return "KC_9";
    case KC_0:         return "KC_0";
    case KC_GRAVE:     return "KC_GRAVE";
    case KC_MINUS:     return "KC_MINUS";
    case KC_PLUS:      return "KC_PLUS";
    case KC_ESC:       return "KC_ESC";
    case KC_F1:        return "KC_F1";
    case KC_F2:        return "KC_F2";
    case KC_F3:        return "KC_F3";
    case KC_F4:        return "KC_F4";
    case KC_F5:        return "KC_F5";
    case KC_F6:        return "KC_F6";
    case KC_F7:        return "KC_F7";
    case KC_F8:        return "KC_F8";
    case KC_F9:        return "KC_F9";
    case KC_F10:       return "KC_F10";
    case KC_F11:       return "KC_F11";
    case KC_F12:       return "KC_F12";
    case KC_A:         return "KC_A";
    case KC_B:         return "KC_B";
    case KC_C:         return "KC_C";
    case KC_D:         return "KC_D";
    case KC_E:         return "KC_E";
    case KC_F:         return "KC_F";
    case KC_G:         return "KC_G";
    case KC_H:         return "KC_H";
    case KC_I:         return "KC_I";
    case KC_J:         return "KC_J";
    case KC_K:         return "KC_K";
    case KC_L:         return "KC_L";
    case KC_M:         return "KC_M";
    case KC_N:         return "KC_N";
    case KC_O:         return "KC_O";
    case KC_P:         return "KC_P";
    case KC_Q:         return "KC_Q";
    case KC_R:         return "KC_R";
    case KC_S:         return "KC_S";
    case KC_T:         return "KC_T";
    case KC_U:         return "KC_U";
    case KC_V:         return "KC_V";
    case KC_W:         return "KC_W";
    case KC_X:         return "KC_X";
    case KC_Y:         return "KC_Y";
    case KC_Z:         return "KC_Z";
    case KC_LEFT:      return "KC_LEFT";
    case KC_UP:        return "KC_UP";
    case KC_DOWN:      return "KC_DOWN";
    case KC_RIGHT:     return "KC_RIGHT";
    }

    return "KC_UNKNOWN";
}

String string_from_enum(MouseButton btn)
{
    switch (btn) {
    case MB_PRIMARY:   return "MB_PRIMARY";
    case MB_SECONDARY: return "MB_SECONDARY";
    case MB_MIDDLE:    return "MB_MIDDLE";
    case MB_WHEEL:     return "MB_MIDDLE";
    case MB_4:         return "MB_4";
    case MB_5:         return "MB_5";
    case MB_UNKNOWN:   return "MB_UNKNOWN";
    case MB_ANY:       return "MB_ANY";
    }

    return "MB_UNKNOWN";
}

#if defined(_WIN32)
#include "win32_window.cpp"
#elif defined(__linux__)
#include "linux_window.cpp"
#else
#error "unsupported platform"
#endif

void init_input_map_(InputMapId *dst, String name, std::initializer_list<InputDesc> descriptors) EXPORT
{
    InputMap map{ .name = name };
    for (auto it : descriptors) {
        if (it.input.type == CHORD) {
            if (it.input.chord.seq && !it.input.chord.length) {
                for (auto *seq = it.input.chord.seq; seq && seq->type; seq++) {
                    it.input.chord.length++;
                }
            }

            auto *seq = it.input.chord.seq;
            it.input.chord.seq = ALLOC_ARR(mem_dynamic, IUnion, it.input.chord.length);
            memcpy(it.input.chord.seq, seq, it.input.chord.length*sizeof *seq);
        }

        if (it.input.device) {
            array_add(&map.by_device[it.input.device][0], it);
            array_add(&map.by_type[it.input.type][it.input.device], it);
        }

        array_add(&map.by_type[it.input.type][0], it);
        if (it.input.device) array_add(&map.by_device[it.input.device][it.input.type], it);

    }

    *dst = array_add(&input.maps, map);
}

void reset_input_map(InputMapId map_id) INTERNAL
{
    if (map_id < 0) return;

    auto *map = &input.maps[map_id];
    for (i32 i = 0; i < map->edges.capacity; i++) map->edges.slots[i].value = 0;

    for (i32 i = AXIS; i <= AXIS_2D; i++) {
        for (auto &it : map->by_device[MOUSE][i]) {
            auto *axis = map_find_emplace(&map->axes, it.id);
            axis[0] = axis[1] = 0;
        }
    }
}

void input_begin_frame() EXPORT
{
    if (auto *it = map_find(&input.mouse, EDGE_DOWN); it) *it = 0;
    if (auto *it = map_find(&input.mouse, EDGE_UP); it) *it = 0;

    reset_input_map(input.active_map);
    for (auto it : input.layers) reset_input_map(it);

    SWAP(input.layers, input.queued_layers);
    input.queued_layers.count = 0;

#ifdef _WIN32
    win32_input_begin_frame();
#endif
}

void set_input_map(InputMapId id) EXPORT
{
    LOG_INFO("switching input map: [%d] %.*s", id, STRFMT(input.maps[id].name));
    reset_input_map(input.active_map);
    input.active_map = id;
    input.layers.count = 0;
}

void push_input_layer(InputMapId layer) EXPORT
{
    PANIC_IF(layer < 0, "Invalid input layer id: %d", layer);
    array_add(&input.queued_layers, layer);
}

InputMapId get_input_map() EXPORT
{
    return input.active_map;
}

bool input_map_active(InputMapId id) EXPORT
{
    if (input.active_map == id) return true;
    for (auto it : input.layers) if (it == id) return true;
    return false;
}

bool translate_input_event(
    DynamicArray<WindowEvent> *queue,
    InputMapId map_id,
    WindowEvent event) INTERNAL
{
    bool handled = false;

    constexpr auto insert_axis_event = [](
        DynamicArray<WindowEvent> *queue,
        InputMapId map,
        InputId id,
        InputType type,
        f32 axis)
    {
        array_insert(queue, 0, { id, .input = { map, type, .axis = axis } });
    };

    constexpr auto insert_axis2d_event = [](
        DynamicArray<WindowEvent> *queue,
        InputMapId map,
        InputId id,
        InputType type,
        f32 axis[2])
    {
        array_insert(queue, 0, { id, .input = { map, type, .axis2d = { axis[0], axis[1] } } });
    };

    constexpr auto insert_input_event = [](
        DynamicArray<WindowEvent> *queue,
        InputMapId map,
        InputId id,
        InputType type)
    {
        array_insert(queue, 0, { id, .input = { map, type } });
    };

    if (event.type < WE_INPUT && map_id != -1) {
        InputMap *map = &input.maps[map_id];

        switch (event.type) {
        case WE_TEXT:
            for (auto &it : map->by_type[TEXT][0]) {
                array_insert(queue, 0, { it.id, .input = { map_id, it.input.type, .text = event.text } });
                handled = handled || !(it.flags & FALLTHROUGH);

                auto *text = map_find_emplace(&map->text, it.id);
                array_add(text, event.text);
            }
            break;
        case WE_PAD_AXIS2:
            for (auto &it : map->by_device[GAMEPAD][AXIS_2D]) {
                if (event.axis2.id == it.input.axis.id) {
                    f32 *axis = map_find_emplace(&map->axes, it.id);
                    axis[0] = event.axis2.value[0];
                    axis[1] = event.axis2.value[1];

                    insert_axis2d_event(queue, map_id, it.id, it.input.type, event.axis2.value);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            } break;
        case WE_PAD_AXIS:
            for (auto &it : map->by_device[GAMEPAD][AXIS]) {
                if (event.axis.id == it.input.axis.id) {
                    f32 *axis = map_find_emplace(&map->axes, it.id);
                    *axis = event.axis.value;

                    insert_axis_event(queue, map_id, it.id, it.input.type, event.axis.value);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_PAD_PRESS:
            for (auto &it : map->by_device[GAMEPAD][EDGE_DOWN]) {
                if (event.pad.button == it.input.pad.button) {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[GAMEPAD][HOLD]) {
                if (event.pad.button == it.input.pad.button) {
                    (*map_find_emplace(&map->active, it, true)) = true;
                    (*map_find_emplace(&map->held, it.id, false)) = true;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_PAD_RELEASE:
            for (auto &it : map->by_device[GAMEPAD][EDGE_UP]) {
                if (event.pad.button == it.input.pad.button) {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[GAMEPAD][HOLD]) {
                if (event.pad.button == it.input.pad.button) {
                    (*map_find_emplace(&map->active, it, false)) = false;
                    (*map_find_emplace(&map->held, it.id, false)) = false;

                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_MOUSE_WHEEL:
            for (auto &it : map->by_device[MOUSE][AXIS]) {
                if (event.mouse_wheel.modifiers == it.input.mouse.modifiers ||
                    it.input.mouse.modifiers == MF_ANY)
                {
                    f32 *axis = map_find_emplace(&map->axes, it.id);
                    axis[0] += event.mouse_wheel.delta;
                    insert_axis_event(queue, map_id, it.id, it.input.type, (f32)event.mouse_wheel.delta);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_MOUSE_MOVE:
            for (auto &it : map->by_device[MOUSE][AXIS_2D]) {
                if ((event.mouse.button == it.input.mouse.button ||
                     it.input.mouse.button == MB_ANY) &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.modifiers == MF_ANY))
                {
                    f32 *axis = map_find_emplace(&map->axes, it.id);
                    axis[0] += event.mouse.dx;
                    axis[1] += event.mouse.dy;
                    insert_axis2d_event(queue, map_id, it.id, it.input.type, (f32[2]){ (f32)event.mouse.dx, (f32)event.mouse.dy });
                    handled = handled || !(it.flags & FALLTHROUGH);

                    if (false) LOG_INFO("[%.*s] mouse move: [%d], final: %d", STRFMT(map->name), it.id, handled);
                }
            }

            for (auto &it : map->by_device[MOUSE][CURSOR]) {
                if ((event.mouse.button == it.input.mouse.button ||
                     it.input.mouse.button == MB_ANY) &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.modifiers == MF_ANY))
                {
                    InputCursor *cursor = map_find_emplace(&map->cursors, it.id);
                    cursor->x = event.mouse.x;
                    cursor->y = event.mouse.y;
                    cursor->dx += event.mouse.dx;
                    cursor->dy += event.mouse.dy;

                    array_insert(queue, 0, {
                        it.id,
                        .input = {
                            map_id,
                            it.input.type,
                            .cursor = {
                                cursor->x, cursor->y,
                                (f32)event.mouse.dx, (f32)event.mouse.dy
                            }}});
                    handled = handled || !(it.flags & FALLTHROUGH);

                    if (false) LOG_INFO("[%.*s] mouse move: [%d], final: %d", STRFMT(map->name), it.id, handled);
                }
            }
            break;
        case WE_MOUSE_PRESS:
            for (auto &it : map->by_device[MOUSE][EDGE_DOWN]) {
                if ((event.mouse.button == it.input.mouse.button ||
                     it.input.mouse.button == MB_ANY) &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.button == MF_ANY))
                {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[MOUSE][HOLD]) {
                if (event.mouse.button == it.input.mouse.button &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.button == MF_ANY))
                {
                    (*map_find_emplace(&map->active, it, true)) = true;
                    (*map_find_emplace(&map->held, it.id, true)) = true;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_MOUSE_RELEASE:
            for (auto &it : map->by_device[MOUSE][EDGE_UP]) {
                if ((event.mouse.button == it.input.mouse.button ||
                     it.input.mouse.button == MB_ANY) &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.modifiers == MF_ANY))
                {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[MOUSE][HOLD]) {
                if ((event.mouse.button == it.input.mouse.button ||
                     it.input.mouse.button == MB_ANY) &&
                    (event.mouse.modifiers == it.input.mouse.modifiers ||
                     it.input.mouse.modifiers == MF_ANY))
                {
                    bool *active = map_find_emplace(&map->active, it, false);
                    if (*active) {
                        *active = false;
                        (*map_find_emplace(&map->held, it.id, false)) = false;
                    }
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }
            break;
        case WE_KEY_PRESS:
            if (event.key.prev_state) break;

            for (auto &it : map->by_type[CHORD][0]) {
                auto &curr = it.input.chord.seq[it.input.chord.at];
                if (curr.device == KEYBOARD && curr.type == EDGE_DOWN) {
                    if (event.key.keycode == curr.key.code &&
                        (event.key.modifiers == curr.key.modifiers ||
                         curr.key.modifiers == MF_ANY))
                    {
                        if (++it.input.chord.at == it.input.chord.length) {
                            (*map_find_emplace(&map->edges, it.id, 0))++;
                            insert_input_event(queue, map_id, it.id, it.input.type);
                            it.input.chord.at = 0;
                        }

                        handled = handled || !(it.flags & FALLTHROUGH);
                    } else {
                        it.input.chord.at = 0;
                    }
                }
            }

            if (handled) break;

            for (auto &it : map->by_device[KEYBOARD][EDGE_DOWN]) {
                if (event.key.keycode == it.input.key.code &&
                    (event.key.modifiers == it.input.key.modifiers ||
                     it.input.key.modifiers == MF_ANY))
                {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[KEYBOARD][HOLD]) {
                if (event.key.keycode == it.input.key.code &&
                    (event.key.modifiers == it.input.key.modifiers ||
                     it.input.key.modifiers == MF_ANY))
                {
                    (*map_find_emplace(&map->active, it, true)) = true;
                    (*map_find_emplace(&map->held, it.id, false)) = true;

                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (i32 i = AXIS; i <= AXIS_2D; i++) {
                for (auto &it : map->by_device[KEYBOARD][i]) {
                    if(event.key.keycode == it.input.key.code &&
                       (event.key.modifiers == it.input.key.modifiers ||
                        it.input.key.modifiers == MF_ANY))
                    {
                        (*map_find_emplace(&map->active, it, true)) = true;
                        f32 *axis = map_find_emplace(&map->axes, it.id);
                        axis[it.input.key.axis] += it.input.key.faxis;

                        insert_axis2d_event(queue, map_id, it.id, it.input.type, (f32[2]){ axis[0], axis[1] });
                        handled = handled || !(it.flags & FALLTHROUGH);
                    }
                }
            }
            break;
        case WE_KEY_RELEASE:
            for (auto &it : map->by_device[KEYBOARD][EDGE_UP]) {
                if (event.key.keycode == it.input.key.code &&
                    (event.key.modifiers == it.input.key.modifiers ||
                     it.input.key.modifiers == MF_ANY))
                {
                    (*map_find_emplace(&map->edges, it.id, 0))++;
                    insert_input_event(queue, map_id, it.id, it.input.type);
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (auto &it : map->by_device[KEYBOARD][HOLD]) {
                if (event.key.keycode == it.input.key.code) {
                    (*map_find_emplace(&map->active, it, true)) = false;
                    (*map_find_emplace(&map->held, it.id, false)) = false;
                    handled = handled || !(it.flags & FALLTHROUGH);
                }
            }

            for (i32 i = AXIS; i <= AXIS_2D; i++) {
                for (auto &it : map->by_device[KEYBOARD][i]) {
                    if (event.key.keycode == it.input.key.code &&
                        (event.key.modifiers == it.input.key.modifiers ||
                         it.input.key.modifiers == MF_ANY))
                    {
                        bool *active = map_find_emplace(&map->active, it, false);
                        if (*active) {
                            *active = false;

                            f32 *axis = map_find_emplace(&map->axes, it.id);
                            axis[it.input.key.axis] -= it.input.key.faxis;
                            insert_axis2d_event(queue, map_id, it.id, it.input.type, (f32[2]){ axis[0], axis[1] });
                        }

                        handled = handled || !(it.flags & FALLTHROUGH);
                    }
                }
            }

            break;
        }

    }

    return handled;
}

bool translate_input_event(
    DynamicArray<WindowEvent> *queue,
    WindowEvent event) INTERNAL
{
    if (false) switch (event.type) {
    case WE_KEY_PRESS:
        LOG_INFO("KEY_PRESS: %.*s [0x%X]; modifiers: [0x%X]; prev_state: %d",
                 STRFMT(string_from_enum((KeyCode_)event.key.keycode)), event.key.keycode,
                 event.key.modifiers,
                 event.key.prev_state);
        break;
    case WE_KEY_RELEASE:
        LOG_INFO("KEY_RELEASE: %.*s [0x%X]; modifiers: [0x%X]; prev_state: %d",
                 STRFMT(string_from_enum((KeyCode_)event.key.keycode)), event.key.keycode,
                 event.key.modifiers,
                 event.key.prev_state);
        break;
    }

    bool handled = false;
    for (auto it : reverse(input.layers)) {
        if (translate_input_event(queue, it, event)) {
            handled = true;
            break;
        }
    }
    handled = handled || translate_input_event(queue, input.active_map, event);

    switch (event.type) {
    case WE_MOUSE_PRESS:
        (*map_find_emplace(&input.mouse, EDGE_DOWN, u8(0))) |= event.mouse.button;
        (*map_find_emplace(&input.mouse, HOLD, u8(0))) |= event.mouse.button;
        break;
    case WE_MOUSE_RELEASE:
        (*map_find_emplace(&input.mouse, EDGE_UP, u8(0))) |= event.mouse.button;
        (*map_find_emplace(&input.mouse, HOLD, u8(0))) &= ~event.mouse.button;

        for (auto &map : input.maps) {
            for (auto &it : map.by_device[MOUSE][HOLD]) {
                if (it.input.key.code == event.key.keycode) {
                    (*map_find_emplace(&map.held, it.id, false)) = false;
                }
            }
            for (auto &it : map.by_device[MOUSE][HOLD]) {
                bool *active = map_find(&map.active, it);
                if (!active || !*active) continue;

                if ((it.input.mouse.button & event.mouse.button) ||
                    it.input.mouse.button == MB_ANY)
                {
                    (*map_find_emplace(&map.held, it.id, false)) = false;
                    *active = false;
                }
            }
        }
        break;
    case WE_KEY_RELEASE:
        for (auto &map : input.maps) {
            for (auto &it : map.by_device[KEYBOARD][HOLD]) {
                bool *active = map_find(&map.active, it);
                if (!active || !*active) continue;

                if (it.input.key.code == event.key.keycode) {
                    (*map_find_emplace(&map.held, it.id, false)) = false;
                    *active = false;
                }
            }

            for (i32 i = AXIS; i < AXIS_2D; i++) {
                for (auto &it : map.by_device[KEYBOARD][i]) {
                    bool *active = map_find(&map.active, it);
                    if (!active || !*active) continue;

                    if (f32 *axis = map_find(&map.axes, it.id);
                        axis && it.input.key.code == event.key.keycode)
                    {
                        axis[it.input.key.axis] -= it.input.key.faxis;
                        *active = false;
                    }
                }
            }
        }
        break;
    default: break;
    }

    return handled;
}

bool text_input_enabled() EXPORT
{
    if (input.active_map != INPUT_MAP_INVALID && input.maps[input.active_map].by_type[TEXT][0].count) return true;
    for (auto it : input.layers) if (input.maps[it].by_type[TEXT][0].count) return true;
    return false;
}

bool get_input_text(InputId id, TextEvent *dst, InputMapId map_id) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_text(id, dst, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    auto *text = map_find(&map->text, id);
    if (!text || text->count == 0) return false;

    *dst = text->at(0);
    array_remove(text, 0);
    return true;
}

bool get_input_axis(InputId id, f32 dst[1], InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_axis(id, dst, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    f32 *axis = map_find(&map->axes, id);
    if (!axis || axis[0] == 0.0f) return false;

    dst[0] = axis[0];
    return true;
}

bool get_input_axis2d(InputId id, f32 dst[2], InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_axis2d(id, dst, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    f32 *axis = map_find(&map->axes, id);
    if (!axis || (axis[0] == 0.0f && axis[1] == 0.0f)) return false;

    dst[0] = axis[0];
    dst[1] = axis[1];
    return true;
}

bool get_input_cursor(InputId id, f32 dst[2], InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_cursor(id, dst, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    InputCursor *cursor = map_find(&map->cursors, id);
    if (!cursor) return false;

    dst[0] = cursor->x;
    dst[1] = cursor->y;
    return true;
}

bool has_input_cursor(InputId id, InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (has_input_cursor(id, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    InputCursor *cursor = map_find(&map->cursors, id);
    if (!cursor) return false;
    return true;
}

bool get_input_cursor_delta(InputId id, f32 dst[2], InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_cursor_delta(id, dst, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    InputCursor *cursor = map_find(&map->cursors, id);
    if (!cursor || (cursor->dx == 0 && cursor->dy == 0)) return false;

    dst[0] = cursor->dx;
    dst[1] = cursor->dy;
    return true;
}

bool has_input_cursor_delta(InputId id, InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (has_input_cursor_delta(id, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    InputCursor *cursor = map_find(&map->cursors, id);
    if (!cursor || (cursor->dx == 0 && cursor->dy == 0)) return false;
    return true;
}

bool get_input_edge(InputId id, InputMapId map_id /*= INPUT_MAP_ANY*/) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers)) if (get_input_edge(id, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    i32 *value = map_find(&map->edges, id);
    if (!value || *value == 0) return false;

    return (*value)-- > 0;
}

bool get_input_held(InputId id, InputMapId map_id) EXPORT
{
    if (map_id == INPUT_MAP_ANY) {
        for (auto it : reverse(input.layers))
            if (get_input_held(id, it)) return true;
        map_id = input.active_map;
    }

    if (map_id == INPUT_MAP_INVALID) return false;
    InputMap *map = &input.maps[map_id];
    bool *value = map_find(&map->held, id);
    if (!value) return false;

    return *value;
}

bool get_input_mouse(MouseButton btn, InputType type /*= EDGE_DOWN*/) EXPORT
{
    auto *it = map_find(&input.mouse, type);
    if (!it) return false;
    return (*it & btn) == btn || (btn == MB_ANY && *it);
}

u32 hash32(const InputDesc &desc, u32 seed /*= MURMUR3_SEED */) EXPORT
{
    u32 state = seed;
    state = hash32(desc.id, state);
    state = hash32(desc.input.device, state);
    state = hash32(desc.input.type, state);

    auto ihash = [](IUnion input, u32 seed) -> u32
    {
        u32 state = seed;

        switch (input.device) {
        case KEYBOARD:
            state = hash32(input.key.code, state);
            state = hash32(input.key.modifiers, state);
            state = hash32(input.key.axis, state);
            state = hash32(input.key.faxis, state);
            break;
        case MOUSE:
            state = hash32(input.mouse.button, state);
            state = hash32(input.mouse.modifiers, state);
            break;
        case GAMEPAD:
            state = hash32(input.pad.button, state);
            break;
        case VAXIS:
            state = hash32(input.axis.id, state);
            break;
        case VTEXT:
            break;
        case IDEVICE_INVALID:
        case IDEVICE_MAX:
            PANIC("invalid device type");
            break;
        }

        return state;
    };

    if (desc.input.type == CHORD) {
        state = hash32(desc.input.chord.length, state);
        for (i32 i = 0; i < desc.input.chord.length; i++) {
            state = ihash(desc.input.chord.seq[i], state);
        }
    } else {
        state = ihash(desc.input, state);
    }

    state = hash32(desc.flags, state);
    return state;

}
