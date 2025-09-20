#ifndef GFX_PUBLIC_H
#define GFX_PUBLIC_H

extern GfxTextureFormat gfx_format_srgb(GfxTextureFormat format);
extern GfxTextureFormat gfx_format_unorm(GfxTextureFormat format);
extern const char *sz_from_enum(GfxTextureFormat format);
extern u32 hash32(const GfxMaterialParameters & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxSamplerDesc & it, u32 seed = MURMUR3_SEED);

#endif // GFX_PUBLIC_H
