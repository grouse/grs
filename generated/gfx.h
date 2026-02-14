#ifndef GFX_GENERATED_H
#define GFX_GENERATED_H

extern GfxTextureFormat gfx_format_srgb(GfxTextureFormat format);
extern GfxTextureFormat gfx_format_unorm(GfxTextureFormat format);
extern const char *sz_from_enum(GfxTextureFormat format);
extern u32 hash32(const GfxMaterialParameters & it, u32 seed = HASH32_SEED);
extern u32 hash32(const GfxSamplerDesc & it, u32 seed = HASH32_SEED);

#endif // GFX_GENERATED_H

#ifdef GFX_GENERATED_IMPL
#define GFX_INTERNAL
#endif
