#ifndef GFX_VULKAN_H
#define GFX_VULKAN_H

#include "gfx.h"

#include "array.h"
#include "map.h"
#include "assets.h"

#include <volk/volk.h>
#define VULKAN_H_ 1
#include <vk_mem_alloc.h>
#include <shaderc/shaderc.h>

constexpr i32 MAX_FRAMES_IN_FLIGHT        = 2;
constexpr i32 MAX_SHADER_STAGES           = 3;
constexpr i32 MAX_COLOR_ATTACHMENTS       = 1;
constexpr i32 MAX_VERTEX_INPUT_BINDINGS   = 4;
constexpr i32 MAX_VERTEX_INPUT_ATTRIBUTES = 4;
constexpr i32 MAX_PUSH_CONSTANTS          = 2;
constexpr i32 MAX_DESCRIPTOR_SETS         = 4;
constexpr i32 MAX_DESCRIPTOR_SET_BINDINGS = 4;

#define VK_CHECK(call)\
    do {\
        VkResult result = call;\
        PANIC_IF(result != VK_SUCCESS, "vulkan fatal error: '%s' [%u] from call '%s': %s", sz_from_enum(result), result, #call);\
    } while (0)

#define VK_IMM SCOPE_EXPR(vk_imm_begin(), vk_imm_end())


struct SpvCompilationResult {
    shaderc_compilation_status status;
    shaderc_compilation_result_t result;
    const char *bytes;
    size_t size;
};

struct GfxVkBuffer {
    VkBuffer handle;
    VmaAllocation allocation;
    VmaAllocationInfo allocation_info;

    i32 size;

    operator VkBuffer() { return handle; }
    bool operator==(const GfxVkBuffer &other) const
    {
        return handle == other.handle && allocation == other.allocation && size == other.size;
    }
};

struct GfxVkTexture {
    VkImage image;
    VkImageView view;
    VkFormat format;

    VmaAllocation allocation;
    VmaAllocationInfo allocation_info;

    explicit operator bool() const { return image != VK_NULL_HANDLE; }
    bool operator==(const GfxVkTexture &other) const
    {
        return image == other.image && view == other.view && allocation == other.allocation;
    }
};

struct GfxVkMaterial {
    GfxPipeline pipeline;
    VkDescriptorSet set;
};

struct GfxVkShader {
    GfxShader handle;

    VkShaderModule module;
    VkShaderStageFlagBits stage;

    operator GfxShader() { return handle; }
    operator VkShaderModule() { return module; }
    explicit operator bool() { return module != VK_NULL_HANDLE; }
};

struct GfxVkPipeline {
    GfxPipeline handle;

    VkPipeline pipeline;
    VkPipelineLayout layout;

    VkDescriptorSetLayout sets[MAX_DESCRIPTOR_SETS];

    operator GfxPipeline() { return handle; }
    operator VkPipeline() { return pipeline; }
    explicit operator bool() { return pipeline != VK_NULL_HANDLE; }
};

struct GfxVkVertexAttributeDesc {
    VkFormat format;
    u32      location;
    u32      offset;
};

struct GfxVkVertexBindingDesc {
    VkVertexInputRate input_rate;
    FixedArray<GfxVkVertexAttributeDesc, MAX_VERTEX_INPUT_ATTRIBUTES> attributes;
    u32 binding;
    u32 stride;
};

struct GfxDescriptorLayoutBindingDesc {
    GfxDescriptorType   type;
    GfxShaderStageFlags stages;
    u32                 count = 1;
    u32                 binding;
};

struct GfxVkPushConstantDesc {
    VkShaderStageFlags stage;
    u32 size;
};

struct GfxVkDescriptorDesc {
    GfxDescriptorType type;
    union {
        struct {
            GfxVkTexture texture;
            VkSampler sampler;
        } texture;
        struct {
            GfxVkTexture *textures;
            VkSampler *samplers;
            i32 count;
        } texture_array;
        GfxVkBuffer uniform;
    };
    u32 binding;

    bool operator==(const GfxVkDescriptorDesc &other) const
    {
        if (binding != other.binding || type != other.type) return false;

        switch (type) {
        case GFX_TEXTURE:
            if (texture.texture != other.texture.texture ||
                texture.sampler != other.texture.sampler)
            {
                return false;
            }
            break;
        case GFX_TEXTURE_ARRAY:
            if (texture_array.count != other.texture_array.count) return false;
            if (texture_array.textures == other.texture_array.textures &&
                texture_array.samplers == other.texture_array.samplers)
            {
                return true;
            }
            for (i32 i = 0; i < texture_array.count; i++) {
                if (texture_array.textures[i] != other.texture_array.textures[i] ||
                    texture_array.samplers[i] != other.texture_array.samplers[i])
                {
                    return false;
                }
            }
            return true;
        case GFX_UNIFORM:
            if (uniform != other.uniform) return false;
            break;
        }

        return true;
    }
};

using GfxVkDescriptorSetLayoutDesc = FixedArray<GfxDescriptorLayoutBindingDesc, MAX_DESCRIPTOR_SET_BINDINGS>;

struct GfxVkDescriptorSetDesc {
    VkPipelineLayout pipeline_layout;
    VkDescriptorSetLayout set_layout;
    FixedArray<GfxVkDescriptorDesc, MAX_DESCRIPTOR_SET_BINDINGS> bindings;

    bool operator==(const GfxVkDescriptorSetDesc &other) const = default;
};

struct GfxVkPipelineDesc {
    FixedArray<VkFormat, MAX_COLOR_ATTACHMENTS> color;

    struct {
        VkFormat format;
        VkBool32 test;
        VkBool32 write;
        VkCompareOp compare_op = VK_COMPARE_OP_GREATER_OR_EQUAL;
    } depth;

    FixedArray<GfxVkShader, MAX_SHADER_STAGES> stages;
    FixedArray<GfxVkVertexBindingDesc, MAX_VERTEX_INPUT_BINDINGS> inputs;
    FixedArray<GfxVkPushConstantDesc, MAX_PUSH_CONSTANTS> push_constants;
    FixedArray<GfxVkDescriptorSetLayoutDesc, MAX_DESCRIPTOR_SETS> descriptor_sets;
    VkPipelineColorBlendAttachmentState blend;
    VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL;
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkCullModeFlags cull_mode = VK_CULL_MODE_NONE;
    VkFrontFace front_face = VK_FRONT_FACE_COUNTER_CLOCKWISE;
};

struct GfxVkFrame {
    VkCommandPool   pool;
    VkCommandBuffer cmd;

    struct {
        GfxVkTexture res;
        u32 index;
    } swapchain;

    struct {
        GfxVkBuffer buffer;
        VkDeviceAddress address;
        i32 offset;
    } vertices;

    VkFence     fence;
    VkSemaphore image_available;
    VkSemaphore render_finished;
};

struct GfxTextureAssetDesc {
    AssetHandle asset;
    bool sRGB;

    bool operator==(const GfxTextureAssetDesc &rhs) const = default;
    bool operator!=(const GfxTextureAssetDesc &rhs) const = default;
};

struct GfxVkAttachment {
    GfxVkTexture res;
    GfxLoadOp load_op;
    GfxStoreOp store_op;
    f32 clear_value[4];
};

struct GfxVkRenderPassDesc {
    GfxVkAttachment color[MAX_COLOR_ATTACHMENTS];
    GfxVkAttachment depth;
    struct { u32 x, y; } offset;
    struct { u32 w, h; } extent;
};

extern struct GfxVkContext {
    VkDevice device;
    VkInstance instance;
    VmaAllocator allocator;

    struct {
        VkPhysicalDevice handle;
        u32 graphics_family_index;
        u32 present_family_index;

        VkPhysicalDeviceVulkan13Features features13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
        VkPhysicalDeviceVulkan12Features features12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &features13 };
        VkPhysicalDeviceVulkan11Features features11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, .pNext = &features12 };
        VkPhysicalDeviceFeatures2 features = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &features11 };
    } physical_device;

    struct {
        VkSwapchainKHR handle;
        VkFormat format;
        VkExtent2D extent;

        Array<VkImage> images;
        Array<VkImageView> views;

        GfxVkTexture depth;
    } swapchain;

    VkSurfaceKHR surface;

    DynamicArray<GfxVkPipelineDesc> pipeline_descs;
    DynamicArray<GfxVkPipeline> pipelines;
    DynamicArray<GfxVkTexture> textures;
    DynamicArray<GfxVkBuffer> buffers;
    DynamicArray<GfxVkMaterial> materials;
    DynamicMap<GfxMaterial, GfxMaterialDesc> material_descs;

    DynamicArray<GfxVkShader> shaders;
    DynamicMap<GfxShader, DynamicArray<GfxPipeline>> shaders_used_by;

    VkDescriptorPool descriptor_pool;
    DynamicArray<VkDescriptorPool> descriptor_pools;

    DynamicMap<VkDescriptorSetLayoutCreateInfo, VkDescriptorSetLayout> descriptor_layouts;
    DynamicMap<GfxVkDescriptorSetDesc, VkDescriptorSet> descriptor_sets;
    DynamicMap<GfxMaterialParameters, GfxVkBuffer> material_parameters;
    DynamicMap<GfxSamplerDesc, VkSampler> samplers;

    VkDebugUtilsMessengerEXT debug_messenger;
    DynamicMap<GfxTextureAssetDesc, GfxTexture>  texture_assets;

    FixedArray<GfxVkFrame, MAX_FRAMES_IN_FLIGHT> frames;
    u32 current_frame = 0;
} vk;


#include "generated/gfx_vulkan.h"

#endif // GFX_VULKAN_H
