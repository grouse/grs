#ifndef GFX_VULKAN_PUBLIC_H
#define GFX_VULKAN_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern GfxTexture gfx_load_texture(String path, bool sRGB = true);
extern GfxTexture gfx_load_texture(AssetHandle handle, bool sRGB = true);

#endif // GFX_VULKAN_PUBLIC_H
