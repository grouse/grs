#include "gfx.h"
#include "hash.h"

const char* sz_from_enum(GfxTextureFormat format) EXPORT
{
    switch (format) {
    case GFX_TEXTURE_R8:              return "GFX_TEXTURE_R8";
    case GFX_TEXTURE_R8G8:            return "GFX_TEXTURE_R8G8";
    case GFX_TEXTURE_R8G8B8:          return "GFX_TEXTURE_R8G8B8";
    case GFX_TEXTURE_R8G8B8A8:        return "GFX_TEXTURE_R8G8B8A8";
    case GFX_TEXTURE_R8_SRGB:         return "GFX_TEXTURE_R8_SRGB";
    case GFX_TEXTURE_R8G8_SRGB:       return "GFX_TEXTURE_R8G8_SRGB";
    case GFX_TEXTURE_R8G8B8_SRGB:     return "GFX_TEXTURE_R8G8B8_SRGB";
    case GFX_TEXTURE_R8G8B8A8_SRGB:   return "GFX_TEXTURE_R8G8B8A8_SRGB";
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
