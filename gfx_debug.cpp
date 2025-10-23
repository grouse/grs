#include "gfx_debug.h"

#include "array.h"

GfxDebugContext gfx_debug;

void gfx_debug_begin_frame() EXPORT
{
    gfx_debug.commands.count = 0;
}

void gfx_debug_draw_sphere(
    Vector3 position,
    f32 radius,
    Vector3 color) EXPORT
{
    array_add(&gfx_debug.commands, {
        .sphere = {
            .type = GFX_DEBUG_SPHERE,
            .color = color,
            .position = position,
            .radius = radius
        }
    });
}

void gfx_debug_draw_cube(
    Vector3 position,
    Vector3 extents,
    Vector3 color) EXPORT
{
    array_add(&gfx_debug.commands, {
        .cube = {
            .type = GFX_DEBUG_CUBE,
            .color = color,
            .center = position,
            .extents = extents,
        }
    });
}

void gfx_debug_draw_line(
    Vector3 p0,
    Vector3 p1,
    f32 thickness,
    Vector3 color,
    f32 duration) EXPORT
{
    array_add(&gfx_debug.commands, {
        .line = {
            .type = GFX_DEBUG_LINE,
            .color = color,
            .p0 = p0, .p1 = p1,
            .thickness = thickness,
        }
    });
}
