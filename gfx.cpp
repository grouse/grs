#include "gfx.h"
#include "hash.h"
#include "assets.h"

#include <stb/stb_image.h>

GfxContext gfx;

void* gfx_load_texture_asset(
    AssetHandle handle,
    void *existing,
    String identifier,
    u8 *data, i32 size)
{
    SArena scratch = tl_scratch_arena();

    int width, height, components;
    if (stbi_info_from_memory(data, size, &width, &height, &components) != 0) {
        if (components == 3) components = 4;
    }

    GfxTextureAsset *asset = (GfxTextureAsset*)existing;
    if (asset) stbi_image_free(asset->data);
    else asset = ALLOC_T(mem_dynamic, GfxTextureAsset) {};

    asset->data   = stbi_load_from_memory(data, size, &width, &height, nullptr, components);
    asset->width  = width;
    asset->height = height;

    switch (components) {
    case 1: asset->format = GFX_TEXTURE_R8_SRGB; break;
    case 2: asset->format = GFX_TEXTURE_R8G8_SRGB; break;
    case 3: asset->format = GFX_TEXTURE_R8G8B8_SRGB; break;
    case 4: asset->format = GFX_TEXTURE_R8G8B8A8_SRGB; break;
    default:
        LOG_ERROR("[vulkan] invalid image component count: %d", components);
        return nullptr;
    }

    return asset;
}

GfxTextureFormat gfx_format_srgb(GfxTextureFormat format) EXPORT
{
    switch (format) {
    case GFX_TEXTURE_R8_UNORM:            return GFX_TEXTURE_R8_SRGB;
    case GFX_TEXTURE_R8G8_UNORM:          return GFX_TEXTURE_R8G8_SRGB;
    case GFX_TEXTURE_R8G8B8_UNORM:        return GFX_TEXTURE_R8G8B8_SRGB;
    case GFX_TEXTURE_R8G8B8A8_UNORM:      return GFX_TEXTURE_R8G8B8A8_SRGB;

    case GFX_TEXTURE_R8_SRGB:
    case GFX_TEXTURE_R8G8_SRGB:
    case GFX_TEXTURE_R8G8B8_SRGB:
    case GFX_TEXTURE_R8G8B8A8_SRGB:
        return format;
    }

    PANIC("[gfx] unhandled format: %s [%d]", sz_from_enum(format), format);
    return format;
}

GfxTextureFormat gfx_format_unorm(GfxTextureFormat format) EXPORT
{
    switch (format) {
    case GFX_TEXTURE_R8_SRGB:            return GFX_TEXTURE_R8_UNORM;
    case GFX_TEXTURE_R8G8_SRGB:          return GFX_TEXTURE_R8G8_UNORM;
    case GFX_TEXTURE_R8G8B8_SRGB:        return GFX_TEXTURE_R8G8B8_UNORM;
    case GFX_TEXTURE_R8G8B8A8_SRGB:      return GFX_TEXTURE_R8G8B8A8_UNORM;
    case GFX_TEXTURE_R8_UNORM:
    case GFX_TEXTURE_R8G8_UNORM:
    case GFX_TEXTURE_R8G8B8_UNORM:
    case GFX_TEXTURE_R8G8B8A8_UNORM:
        return format;
    }

    PANIC("[gfx] unhandled format: %s [%d]", sz_from_enum(format), format);
    return format;
}


const char* sz_from_enum(GfxTextureFormat format) EXPORT
{
    switch (format) {
    case GFX_TEXTURE_R8_UNORM:       return "GFX_TEXTURE_R8_UNORM";
    case GFX_TEXTURE_R8G8_UNORM:     return "GFX_TEXTURE_R8G8_UNORM";
    case GFX_TEXTURE_R8G8B8_UNORM:   return "GFX_TEXTURE_R8G8B8_UNORM";
    case GFX_TEXTURE_R8G8B8A8_UNORM: return "GFX_TEXTURE_R8G8B8A8_UNORM";
    case GFX_TEXTURE_R8_SRGB:        return "GFX_TEXTURE_R8_SRGB";
    case GFX_TEXTURE_R8G8_SRGB:      return "GFX_TEXTURE_R8G8_SRGB";
    case GFX_TEXTURE_R8G8B8_SRGB:    return "GFX_TEXTURE_R8G8B8_SRGB";
    case GFX_TEXTURE_R8G8B8A8_SRGB:  return "GFX_TEXTURE_R8G8B8A8_SRGB";
    }

    LOG_ERROR("[gfx] unknown GfxTextureFormat: %d", format);
    return "unknown";
}

u32 hash32(const GfxMaterialParameters &it, u32 seed /*= HASH32_SEED*/) EXPORT
{
    h32s state = hash32_start(seed);
    hash32_update(&state, &it.albedo_factor, sizeof it.albedo_factor);
    hash32_update(&state, &it.metallic_roughness_factor, sizeof it.metallic_roughness_factor);
    hash32_update(&state, it.alpha_cutoff);
    hash32_update(&state, it.normal_scale);
    return hash32_digest(&state);
}

u32 hash32(const GfxSamplerDesc &it, u32 seed /*= HASH32_SEED*/) EXPORT
{
    h32s state = hash32_start(seed);
    hash32_update(&state, it.min_filter);
    hash32_update(&state, it.mag_filter);
    hash32_update(&state, it.wrap_u);
    hash32_update(&state, it.wrap_v);
    hash32_update(&state, it.border_color);
    return hash32_digest(&state);
}
