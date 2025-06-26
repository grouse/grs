#ifndef GFX_H
#define GFX_H

#include "core.h"
#include "maths.h"
#include "hash.h"

#define GFX_HANDLE(name, underlying_t)\
    struct name {\
        underlying_t idx;\
        constexpr name(underlying_t val) : idx(val) {}\
        constexpr name() : idx(0) {}\
        operator underlying_t&() { return idx; }\
        operator const underlying_t&() const { return idx; }\
        constexpr explicit operator bool() { return idx != 0; }\
    };\
    constexpr name name##_INVALID = 0

GFX_HANDLE(GfxTexture, u32);
GFX_HANDLE(GfxBuffer, u32);
GFX_HANDLE(GfxPipeline, u32);
GFX_HANDLE(GfxShader, u32);
GFX_HANDLE(GfxMaterial, u64);

enum GfxShaderStageFlagsBits {
    GFX_VERTEX   = 1 << 0,
    GFX_FRAGMENT = 1 << 1,
    GFX_GEOMETRY = 1 << 2,
};
typedef u32 GfxShaderStageFlags;

enum GfxTextureFormat {
    GFX_TEXTURE_R8_UNORM,
    GFX_TEXTURE_R8G8_UNORM,
    GFX_TEXTURE_R8G8B8_UNORM,
    GFX_TEXTURE_R8G8B8A8_UNORM,

    GFX_TEXTURE_R8_SRGB,
    GFX_TEXTURE_R8G8_SRGB,
    GFX_TEXTURE_R8G8B8_SRGB,
    GFX_TEXTURE_R8G8B8A8_SRGB,
};

enum GfxSwizzle {
    GFX_SWIZZLE_IDENTITY,
    GFX_SWIZZLE_RRRR,
    GFX_SWIZZLE_XXXR,
};

enum GfxDescriptorType {
    GFX_TEXTURE,
    GFX_TEXTURE_ARRAY,
    GFX_UNIFORM,
};

struct GfxTextureAsset {
    void *data;
    u32 width, height;
    GfxTextureFormat format;
};


struct GfxMaterialParameters {
    Vector4 albedo_factor             = {1, 1, 1, 1};
    Vector4 metallic_roughness_factor = {1, 1, 0, 0 };
    f32     alpha_cutoff              = 0.0f;
    f32     normal_scale              = 1.0f;

    bool operator==(const GfxMaterialParameters &other) const = default;
};

enum GfxSampleWrap {
    GFX_WRAP_REPEAT = 0,
    GFX_WRAP_MIRROR,
    GFX_WRAP_CLAMP,
};

enum GfxSampleFilter {
    GFX_FILTER_NEAREST = 0,
    GFX_FILTER_LINEAR,
};

enum GfxSampleBorderColor {
    GFX_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK = 0,
    GFX_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
};

struct GfxSamplerDesc {
    GfxSampleWrap wrap_u;
    GfxSampleWrap wrap_v;
    GfxSampleFilter mag_filter;
    GfxSampleFilter min_filter;
    GfxSampleBorderColor border_color;

    bool operator==(const GfxSamplerDesc &other) const = default;
};

struct GfxMaterialDesc {
    GfxTexture albedo;
    GfxTexture metallic_roughness;
    GfxTexture normal;

    GfxSamplerDesc albedo_sampler;
    GfxSamplerDesc metallic_roughness_sampler;
    GfxSamplerDesc normal_sampler;

    GfxMaterialParameters parameters;
};

struct MeshVertex {
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    Vector4 tangent;
};

struct GfxMesh {
    GfxBuffer vertex_buffer;
    GfxBuffer index_buffer;
    i32 index_count;
    i32 first_index;
    i32 vertex_offset;

    explicit operator bool() { return vertex_buffer != GfxBuffer_INVALID; }
};

#include "generated/gfx.h"

Vector2 gfx_resolution();

GfxTexture gfx_create_texture(GfxTextureFormat format, GfxSwizzle swizzle, u32 width, u32 height);
GfxTexture gfx_create_texture(void *pixels, GfxTextureFormat format, GfxSwizzle swizzle, u32 width, u32 height);
void gfx_copy_texture(GfxTexture dst, u32 dst_x, u32 dst_y, u32 w, u32 h, const void *pixels);

GfxMaterial gfx_create_material(GfxMaterialDesc desc);

GfxBuffer gfx_create_vertex_buffer(void *data, i32 size);
GfxBuffer gfx_create_index_buffer(void *data, i32 size);

Vector4 cs_from_fs(Vector4 fs_v);
Vector2 ndc_from_fs(Vector2 fs_v);
Vector3 cs_from_ss(Vector3 ss_v);

#endif // GFX_H
