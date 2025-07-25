#ifndef GFX_VULKAN_PUBLIC_H
#define GFX_VULKAN_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern Vector2 gfx_resolution();
extern void gfx_wait_frame();
extern GfxBuffer gfx_create_buffer(i32 size);
extern GfxBuffer gfx_create_buffer(void *data, i32 size);
extern GfxBuffer gfx_create_vertex_buffer(void *data, i32 size);
extern GfxBuffer gfx_create_index_buffer(void *data, i32 size);
extern GfxTexture gfx_load_texture(String path, bool sRGB = true);
extern GfxTexture gfx_load_texture(AssetHandle handle, bool sRGB = true);
extern void gfx_begin_pass(const GfxVkRenderPassDesc & desc);
extern void gfx_end_pass();
extern const char *sz_from_enum(GfxLoadOp op);
extern const char *sz_from_enum(GfxStoreOp op);

#endif // GFX_VULKAN_PUBLIC_H
