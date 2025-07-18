#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"
#include "string.h"
#include "maths.h"
#include "hash.h"

#include <initializer_list>

#define init_input_map(id, ...) init_input_map_(&id, #id, __VA_ARGS__)

enum WindowEventType : u32 {
    WE_INVALID = 0,
    WE_MOUSE_WHEEL,
    WE_MOUSE_PRESS,
    WE_MOUSE_RELEASE,
    WE_MOUSE_MOVE,

    WE_KEY_PRESS,
    WE_KEY_RELEASE,

    WE_PAD_CONNECT,
    WE_PAD_DISCONNECT,
    WE_PAD_PRESS,
    WE_PAD_RELEASE,

    WE_PAD_AXIS,
    WE_PAD_AXIS2,

    WE_TEXT,

    WE_RESIZE,
    WE_QUIT,

    WE_INPUT,
};

enum MouseCursor {
    MC_NORMAL = 0,
    MC_SIZE_NW_SE,
    MC_HIDDEN,
    MC_MAX,
};

enum GamepadButton : u8 {
    PAD_F_UP,
    PAD_F_DOWN,
    PAD_F_RIGHT,
    PAD_F_LEFT,

    PAD_D_UP,
    PAD_D_DOWN,
    PAD_D_LEFT,
    PAD_D_RIGHT,

    PAD_LB, PAD_L1 = PAD_LB,
    PAD_LS, PAD_L3 = PAD_LS,

    PAD_RB, PAD_R1 = PAD_RB,
    PAD_RS, PAD_R3 = PAD_RS,
};

enum GamepadAxis : u8 {
    PAD_TRIG0,
    PAD_TRIG1,

    PAD_JOY0,
    PAD_JOY1,
};

enum KeyCode_ : u8 {
    KC_UNKNOWN = 0,

    KC_BACKSPACE,
    KC_ENTER,
    KC_TAB,

    KC_INSERT,
    KC_DELETE,
    KC_HOME,
    KC_END,
    KC_PAGE_UP,
    KC_PAGE_DOWN,
    KC_SCLK,
    KC_BREAK,

    KC_SPACE,

    KC_SHIFT,
    KC_CTRL,
    KC_ALT,
    KC_LSUPER,
    KC_RSUPER,
    KC_APP,

    KC_LBRACKET,  // [{ on US/UK
    KC_RBRACKET, // ]} on US/UK
    KC_BACKSLASH,
    KC_COLON,
    KC_TICK,
    KC_COMMA,
    KC_PERIOD,
    KC_SLASH,

    KC_1,
    KC_2,
    KC_3,
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    KC_0,

    KC_GRAVE,
    KC_MINUS,
    KC_PLUS,

    KC_ESC,

    KC_F1,
    KC_F2,
    KC_F3,
    KC_F4,
    KC_F5,
    KC_F6,
    KC_F7,
    KC_F8,
    KC_F9,
    KC_F10,
    KC_F11,
    KC_F12,

    KC_A,
    KC_B,
    KC_C,
    KC_D,
    KC_E,
    KC_F,
    KC_G,
    KC_H,
    KC_I,
    KC_J,
    KC_K,
    KC_L,
    KC_M,
    KC_N,
    KC_O,
    KC_P,
    KC_Q,
    KC_R,
    KC_S,
    KC_T,
    KC_U,
    KC_V,
    KC_W,
    KC_X,
    KC_Y,
    KC_Z,

    KC_LEFT,
    KC_UP,
    KC_DOWN,
    KC_RIGHT,
};

enum ModifierFlags : u8 {
    MF_ALT   = 1 << 0,
    MF_CTRL  = 1 << 1,
    MF_SHIFT = 1 << 2,

    MF_ANY   = 0xFF,
};

enum MouseButton : u8 {
    MB_UNKNOWN   = 0,

    MB_PRIMARY   = 1 << 0,
    MB_SECONDARY = 1 << 1,
    MB_MIDDLE    = 1 << 2,
    MB_WHEEL     = 1 << 3,

    MB_4         = 1 << 4,
    MB_5         = 1 << 5,

    MB_ANY       = 0xFF,
};

enum InputType {
    ITYPE_INVALID = 0,

    AXIS = 1,
    AXIS_2D,

    CURSOR,

    EDGE_DOWN,
    EDGE_UP,
    HOLD,

    CHORD,

    TEXT,

    ITYPE_MAX,
};

enum InputDevice {
    IDEVICE_INVALID = 0,
    VCHORD = 0,

    KEYBOARD,
    MOUSE,

    GAMEPAD,

    VTEXT,
    VAXIS,

    IDEVICE_MAX,
};

enum InputFlags {
    FALLTHROUGH = 1 << 0,
};

enum {
    CONFIRM = 0xF000,
    CANCEL,

    SAVE,

    TEXT_INPUT,

    UNDO,
    REDO,

    DUPLICATE,
    COPY,
    PASTE,
    CUT,

    DELETE,

    SELECT,
    SELECT_ALL,
    DESELECT,

    FORWARD,
    BACK
};

enum WindowFlags : u32 {
    WINDOW_OPENGL = 1 << 0,
    WINDOW_VULKAN = 1 << 1,
};

struct AppWindow;

typedef u32 InputId;
typedef i32 InputMapId;

constexpr i32 INPUT_MAP_INVALID = -1;
constexpr i32 INPUT_MAP_ANY = -2;

struct MouseState {
    i16 x, y;
    bool captured;
};

extern MouseState g_mouse;

struct MouseEvent {
    u8 modifiers;
    u8 button;
    i16 x, y;
    i16 dx, dy;
};

struct WindowCreateDesc {
    String title;
    i32 width, height;
    u32 flags = WINDOW_OPENGL;
};

struct IKey {
    InputDevice device = KEYBOARD;
    InputType   type = EDGE_DOWN;

    u8 code;
    u8 modifiers;
    u8 axis;
    i8 faxis;
};

struct IMouse {
    InputDevice device = MOUSE;
    InputType   type = EDGE_DOWN;

    u8 button;
    u8 modifiers;
};

struct IPad {
    InputDevice device = GAMEPAD;
    InputType   type = EDGE_DOWN;
    u8 button;
};

struct IAxis {
    InputDevice device = VAXIS;
    InputType   type = AXIS;
    u8 id;
};

struct IText {
    InputDevice device = VTEXT;
    InputType   type = TEXT;
};

struct IChord {
    InputDevice device = VCHORD;
    InputType   type = CHORD;
    union IUnion *seq;
    i32 length, at;
};

union IUnion {
    struct {
        InputDevice device;
        InputType   type;
    };

    IKey   key;
    IMouse mouse;
    IPad   pad;
    IAxis  axis;
    IText  text;
    IChord chord;
};


#define IKEY(...)   IUnion{ .key   = { KEYBOARD, EDGE_DOWN, __VA_ARGS__ } }
#define IMOUSE(...) IUnion{ .mouse = { MOUSE, CURSOR, __VA_ARGS__ } }
#define IAXIS(...)  IUnion{ .axis  = { __VA_ARGS__ } }
#define ITEXT()     IUnion{ .text  = { VTEXT, TEXT } }
#define ICHORD(...) IUnion{ .chord = { VCHORD, CHORD, (IUnion[]){ __VA_ARGS__, {} } } }
#define IKAXIS(...) IUnion{ .key   = { KEYBOARD, AXIS, __VA_ARGS__ } }
#define IPAD(...)   IUnion{ .pad   = { GAMEPAD, EDGE_DOWN, __VA_ARGS__ } }


struct InputDesc {
    InputId id;
    IUnion  input;
    u32     flags;

    bool operator!=(const InputDesc &rhs)
    {
        if (id != rhs.id || input.device != rhs.input.device || input.type != rhs.input.type || flags != rhs.flags)
            return true;

        switch (input.device) {
        case KEYBOARD:
            return
                input.key.code != rhs.input.key.code ||
                input.key.modifiers != rhs.input.key.modifiers ||
                input.key.axis != rhs.input.key.axis ||
                input.key.faxis != rhs.input.key.faxis;
        case MOUSE:
            return
                input.mouse.button != rhs.input.mouse.button ||
                input.mouse.modifiers != rhs.input.mouse.modifiers;
        case GAMEPAD:
            return input.pad.button != rhs.input.pad.button;
        case VAXIS:
            return input.axis.id != rhs.input.axis.id;
        case VTEXT:
            break;
        case IDEVICE_INVALID:
        case IDEVICE_MAX:
            PANIC("invalid device type");
            break;
        }

        return false;
    }
};

struct TextEvent {
    // NOTE(jesper): key-repeat fills this buffer with repeated utf8 sequences
    u8 modifiers;
    u8 c[12];
    u8 length;
};

struct WindowEvent {
    u32 type;
    union {
        struct {
            u8 modifiers;
            i16 delta;
        } mouse_wheel;
        MouseEvent mouse;
        struct {
            u32 modifiers  : 8;
            u32 keycode    : 8;
            u32 prev_state : 1;
            u32 repeat     : 1;
            u32 unused     : 14;
        } key;
        struct {
            u32 button     : 8;
            u32 unused     : 24;
        } pad;
        struct {
            u8  id;
            f32 value;
        } axis;
        struct {
            u8 id;
            f32 value[2];
        } axis2;
        TextEvent text;
        struct {
            InputMapId map;
            InputType type;
            union {
                f32 axis;
                f32 axis2d[2];
                struct {
                    f32 x, y;
                    f32 dx, dy;
                } cursor;
                TextEvent text;
            };
        } input;
        struct {
            i16 width, height;
        } resize;
    };
};

#include "generated/window.h"

AppWindow* create_window(WindowCreateDesc desc);
void present_window(AppWindow *wnd);

Vector2 get_client_resolution(AppWindow *wnd);

void wait_for_next_event(AppWindow *wnd);
bool next_event(AppWindow *wnd, WindowEvent *dst);

void capture_mouse(AppWindow *wnd);
void release_mouse(AppWindow *wnd);

void push_cursor(MouseCursor c);

#endif // WINDOW_H
