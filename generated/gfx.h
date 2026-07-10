#ifndef GFX_GENERATED_H
#define GFX_GENERATED_H

extern Vector2 gfx_resolution();
extern GfxTexture gfx_create_texture(GfxTextureFormat format, GfxSwizzle swizzle, u32 width, u32 height);
extern GfxTexture gfx_create_texture(void *pixels, GfxTextureFormat format, GfxSwizzle swizzle, u32 width, u32 height);
extern void gfx_copy_texture(GfxTexture dst, u32 dst_x, u32 dst_y, u32 w, u32 h, const void *pixels);
extern GfxMaterialIdx gfx_material(GfxMaterial desc);
extern AssetHandle gfx_get_texture_asset(GfxTexture texture);
extern GfxMesh gfx_create_mesh(Array<MeshVertex> vertices, Array<u32> indices, i32 index_count);
extern GfxBuffer gfx_create_vertex_buffer(void *data, i32 size);
extern GfxBuffer gfx_create_index_buffer(void *data, i32 size);
extern Vector4 cs_from_fs(Vector4 fs_v);
extern Vector2 ndc_from_fs(Vector2 fs_v);
extern Vector3 cs_from_ss(Vector3 ss_v);
extern void *gfx_load_texture_asset(AssetHandle handle, void *existing, String identifier, u8 *data, i32 size);
extern GfxTextureFormat gfx_format_srgb(GfxTextureFormat format);
extern GfxTextureFormat gfx_format_unorm(GfxTextureFormat format);
extern const char *sz_from_enum(GfxTextureFormat format);
extern u32 hash32(const GfxMaterialParameters & it, u32 seed = HASH32_SEED);
extern u32 hash32(const GfxSampler & it, u32 seed = HASH32_SEED);

#endif // GFX_GENERATED_H

#ifdef GFX_GENERATED_IMPL
#define GFX_INTERNAL
#endif
