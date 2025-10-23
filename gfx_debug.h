#ifndef GFX_DEBUG_H
#define GFX_DEBUG_H

#include "core.h"
#include "maths.h"
#include "array.h"

#define GFX_MAX_DEBUG_COMMANDS 255

enum GfxDebugType {
    GFX_DEBUG_CUBE,
    GFX_DEBUG_SPHERE,
    GFX_DEBUG_LINE,
};

struct GfxDebugCommand {
    union {
        struct {
            GfxDebugType type;
            Vector3 color;
        };

        struct {
            GfxDebugType type;
            Vector3 color;
            Vector3 center;
            Vector3 extents;
        } cube;

        struct {
            GfxDebugType type;
            Vector3 color;
            Vector3 position;
            f32 radius;
        } sphere;

        struct {
            GfxDebugType type;
            Vector3 color;
            Vector3 p0, p1;
            f32 thickness;
        } line;
    };
};

struct GfxDebugContext {
    FixedArray<GfxDebugCommand, GFX_MAX_DEBUG_COMMANDS> commands;
};

extern GfxDebugContext gfx_debug;

#include "generated/gfx_debug.h"

#endif // GFX_DEBUG_H
