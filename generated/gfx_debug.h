#ifndef GFX_DEBUG_GENERATED_H
#define GFX_DEBUG_GENERATED_H

extern void gfx_debug_begin_frame();
extern void gfx_debug_draw_sphere(Vector3 position, f32 radius, Vector3 color);
extern void gfx_debug_draw_cube(Vector3 position, Vector3 extents, Vector3 color);
extern void gfx_debug_draw_line(Vector3 p0, Vector3 p1, f32 thickness, Vector3 color, f32 duration);

#endif // GFX_DEBUG_GENERATED_H
