#include "gfx.h"
#include "hash.h"
#include "assets.h"

#include <stb/stb_image.h>

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

    GfxTextureFormat format = GFX_TEXTURE_R8_SRGB;

    switch (components) {
    case 1: format = GFX_TEXTURE_R8_SRGB; break;
    case 2: format = GFX_TEXTURE_R8G8_SRGB; break;
    case 3: format = GFX_TEXTURE_R8G8B8_SRGB; break;
    case 4: format = GFX_TEXTURE_R8G8B8A8_SRGB; break;
    default:
        LOG_ERROR("[vulkan][%.*s] invalid image component count: %d", STRFMT(identifier), components);
        return nullptr;
    }

    stbi_uc *pixels = stbi_load_from_memory(data, size, &width, &height, nullptr, components);


    GfxTextureAsset *asset = (GfxTextureAsset*)existing;
    if (asset) stbi_image_free(asset->data);
    else asset = ALLOC_T(mem_dynamic, GfxTextureAsset) {};

    asset->data   = pixels;
    asset->width  = width;
    asset->height = height;
    asset->format = format;
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

u32 hash32(const GfxMaterialParameters &it, u32 seed /*= MURMUR3_SEED*/) EXPORT
{
    u32 state = seed;
    state = hash32(&it.albedo_factor, sizeof it.albedo_factor, state);
    state = hash32(&it.metallic_roughness_factor, sizeof it.metallic_roughness_factor, state);
    state = hash32(it.alpha_cutoff, state);
    state = hash32(it.normal_scale, state);
    return state;
}

u32 hash32(const GfxSamplerDesc &it, u32 seed /*= MURMUR3_SEED*/) EXPORT
{
    u32 state = seed;
    state = hash32(it.min_filter, state);
    state = hash32(it.mag_filter, state);
    state = hash32(it.wrap_u, state);
    state = hash32(it.wrap_v, state);
    state = hash32(it.border_color, state);
    return state;
}
