#include "gfx_vulkan.h"
#include "generated/internal/gfx_vulkan.h"
#include "gfx.h"

GfxVkContext vk;

extern void vk_set_viewport(VkCommandBuffer cmd, f32 width, f32 height) INTERNAL
{
    VkViewport viewport = { .width = width, .height = height, .minDepth = 0.0f, .maxDepth = 1.0f };
    vkCmdSetViewport(cmd, 0, 1, &viewport);
}

extern void vk_set_scissor(VkCommandBuffer cmd, f32 width, f32 height) INTERNAL
{
    VkRect2D scissor{ .extent = { u32(width), u32(height) } };
    vkCmdSetScissor(cmd, 0, 1, &scissor);
}

extern void vk_transition_image(
    VkCommandBuffer cmd,
    GfxVkTexture texture,
    VkImageLayout old_layout,
    VkImageLayout new_layout) INTERNAL
{
    VkImageAspectFlags aspect = vk_aspect_mask(texture.format);
    vk_transition_image(cmd, texture.image, aspect, old_layout, new_layout);
}

extern void vk_transition_image(
    VkCommandBuffer cmd,
    VkImage image,
    VkImageAspectFlags aspect,
    VkImageLayout old_layout,
    VkImageLayout new_layout) INTERNAL
{
    VkImageMemoryBarrier2 barrier{
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

        .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,

        .oldLayout = old_layout,
        .newLayout = new_layout,

        .image = image,

        .subresourceRange = {
            .aspectMask     = aspect,
            .baseMipLevel   = 0, .levelCount = VK_REMAINING_MIP_LEVELS,
            .baseArrayLayer = 0, .layerCount = VK_REMAINING_ARRAY_LAYERS,
        },
    };

    VkDependencyInfo dep_info{
        VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &barrier,
    };

    vkCmdPipelineBarrier2(cmd, &dep_info);
}

extern GfxVkBuffer vk_create_buffer(
    i32 size,
    VkBufferUsageFlags usage,
    VmaMemoryUsage mem_usage,
    VmaAllocationCreateFlagBits flags) INTERNAL
{
    if (!size) return {};

    VkBufferCreateInfo buffer_info{
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = (VkDeviceSize)size,
        .usage = usage,
    };

    VmaAllocationCreateInfo alloc_info{
        .flags = (VmaAllocationCreateFlags)flags,
        .usage = mem_usage,
    };

    GfxVkBuffer buffer{ .size = size };
    VK_CHECK(vmaCreateBuffer(vk.allocator, &buffer_info, &alloc_info, &buffer.handle, &buffer.allocation, &buffer.allocation_info));
    return buffer;
}

extern GfxVkBuffer vk_create_buffer(
    void *data, i32 size,
    VkBufferUsageFlags usage,
    VmaMemoryUsage mem_usage,
    VmaAllocationCreateFlagBits flags) INTERNAL
{
    GfxVkBuffer buffer = vk_create_buffer(size, usage, mem_usage, flags);
    vmaMapMemory(vk.allocator, buffer.allocation, &buffer.allocation_info.pMappedData);
    memcpy(buffer.allocation_info.pMappedData, data, size);
    vmaUnmapMemory(vk.allocator, buffer.allocation);
    return buffer;
}

extern void vk_destroy_buffer(GfxVkBuffer buffer) INTERNAL
{
    vmaDestroyBuffer(vk.allocator, buffer.handle, buffer.allocation);
}

extern VkImageAspectFlags vk_aspect_mask(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
        return VK_IMAGE_ASPECT_DEPTH_BIT;

    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    case VK_FORMAT_S8_UINT:
        return VK_IMAGE_ASPECT_STENCIL_BIT;

    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

extern VkFormat vk_format(GfxTextureFormat format) INTERNAL
{
    switch (format) {
    case GFX_TEXTURE_R8:            return VK_FORMAT_R8_UNORM;
    case GFX_TEXTURE_R8_SRGB:       return VK_FORMAT_R8_SRGB;
    case GFX_TEXTURE_R8G8:          return VK_FORMAT_R8G8_UNORM;
    case GFX_TEXTURE_R8G8_SRGB:     return VK_FORMAT_R8G8_SRGB;
    case GFX_TEXTURE_R8G8B8:        return VK_FORMAT_R8G8B8_UNORM;
    case GFX_TEXTURE_R8G8B8_SRGB:   return VK_FORMAT_R8G8B8_SRGB;
    case GFX_TEXTURE_R8G8B8A8:      return VK_FORMAT_R8G8B8A8_UNORM;
    case GFX_TEXTURE_R8G8B8A8_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
    }

    LOG_ERROR("unsupported texture format: %d", format);
    return VK_FORMAT_UNDEFINED;
}

extern u32 vk_format_size(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_R32G32_SFLOAT: return 2*sizeof(f32);
    case VK_FORMAT_R32G32B32_SFLOAT: return 3*sizeof(f32);
    case VK_FORMAT_R32G32B32A32_SFLOAT: return 4*sizeof(f32);
    default:
       PANIC("unhandled VkFormat size '%s' [%d]", sz_from_enum(format), format);
       return 0;
    }
}

extern u32 vk_block_size(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_R8_UNORM:       return 1;
    case VK_FORMAT_R8_SRGB:        return 1;
    case VK_FORMAT_R8G8B8_UNORM:   return 3;
    case VK_FORMAT_R8G8B8_SRGB:    return 3;
    case VK_FORMAT_R8G8B8A8_UNORM: return 4;
    case VK_FORMAT_R8G8B8A8_SRGB:  return 4;
    default:
        LOG_ERROR("[gfx] unsupported format: %s", sz_from_enum(format));
        return 0;
    }
}

extern VkSamplerAddressMode vk_wrap_mode(GfxSampleWrap wrap) INTERNAL
{
    switch (wrap) {
    case GFX_WRAP_REPEAT: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case GFX_WRAP_MIRROR: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case GFX_WRAP_CLAMP: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }

    LOG_ERROR("[gfx] unknown wrap mode: %d", wrap);
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

extern VkBorderColor vk_border_color(GfxSampleBorderColor border_color) INTERNAL
{
    switch (border_color) {
    case GFX_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
        return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    case GFX_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
        return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    }

    LOG_ERROR("[gfx] unknown border color: %d", border_color);
    return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
}

extern VkFilter vk_filter(GfxSampleFilter filter) INTERNAL
{
    switch (filter) {
    case GFX_FILTER_NEAREST: return VK_FILTER_NEAREST;
    case GFX_FILTER_LINEAR: return VK_FILTER_LINEAR;
    }

    LOG_ERROR("[gfx] unknown filter mode: %d", filter);
    return VK_FILTER_LINEAR;
}



extern VkComponentMapping vk_component_mapping(GfxSwizzle swizzle) INTERNAL
{
    VkComponentMapping components{};

    switch (swizzle) {
    case GFX_SWIZZLE_IDENTITY:
        break;
    case GFX_SWIZZLE_XXXR:
        components.a = VK_COMPONENT_SWIZZLE_R;
        break;
    case GFX_SWIZZLE_RRRR:
        components.r = VK_COMPONENT_SWIZZLE_R;
        components.g = VK_COMPONENT_SWIZZLE_R;
        components.b = VK_COMPONENT_SWIZZLE_R;
        components.a = VK_COMPONENT_SWIZZLE_R;
        break;
    }

    return components;
}

extern const char* sz_from_enum(shaderc_compilation_status status) INTERNAL
{
    switch (status) {
    case shaderc_compilation_status_success: return "success";
    case shaderc_compilation_status_invalid_stage: return "invalid_stage";
    case shaderc_compilation_status_compilation_error: return "compilation_error";
    case shaderc_compilation_status_internal_error: return "internal_error";
    case shaderc_compilation_status_null_result_object: return "null_result_object";
    case shaderc_compilation_status_invalid_assembly: return "invalid_assembly";
    case shaderc_compilation_status_validation_error: return "validation_error";
    case shaderc_compilation_status_transformation_error: return "transformation_error";
    case shaderc_compilation_status_configuration_error: return "configuration_error";
    }

    return "unknown";
}

extern const char* sz_from_enum(VkResult result) INTERNAL
{
    switch (result) {
    case VK_SUCCESS: return "VK_SUCCESS";
    case VK_NOT_READY: return "VK_NOT_READY";
    case VK_TIMEOUT: return "VK_TIMEOUT";
    case VK_EVENT_SET: return "VK_EVENT_SET";
    case VK_EVENT_RESET: return "VK_EVENT_RESET";
    case VK_INCOMPLETE: return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
    case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_PIPELINE_COMPILE_REQUIRED: return "VK_PIPELINE_COMPILE_REQUIRED";
    case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_NOT_PERMITTED_KHR: return "VK_ERROR_NOT_PERMITTED_KHR";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
    case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
    case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
    case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
    case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT: return "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";
    case VK_RESULT_MAX_ENUM: return "VK_RESULT_MAX_ENUM";
    }

    return "unknown";
}

extern const char* sz_from_enum(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_UNDEFINED: return "VK_FORMAT_UNDEFINED";
    case VK_FORMAT_R4G4_UNORM_PACK8: return "VK_FORMAT_R4G4_UNORM_PACK8";
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    case VK_FORMAT_R5G6B5_UNORM_PACK16: return "VK_FORMAT_R5G6B5_UNORM_PACK16";
    case VK_FORMAT_B5G6R5_UNORM_PACK16: return "VK_FORMAT_B5G6R5_UNORM_PACK16";
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    case VK_FORMAT_R8_UNORM: return "VK_FORMAT_R8_UNORM";
    case VK_FORMAT_R8_SNORM: return "VK_FORMAT_R8_SNORM";
    case VK_FORMAT_R8_USCALED: return "VK_FORMAT_R8_USCALED";
    case VK_FORMAT_R8_SSCALED: return "VK_FORMAT_R8_SSCALED";
    case VK_FORMAT_R8_UINT: return "VK_FORMAT_R8_UINT";
    case VK_FORMAT_R8_SINT: return "VK_FORMAT_R8_SINT";
    case VK_FORMAT_R8_SRGB: return "VK_FORMAT_R8_SRGB";
    case VK_FORMAT_R8G8_UNORM: return "VK_FORMAT_R8G8_UNORM";
    case VK_FORMAT_R8G8_SNORM: return "VK_FORMAT_R8G8_SNORM";
    case VK_FORMAT_R8G8_USCALED: return "VK_FORMAT_R8G8_USCALED";
    case VK_FORMAT_R8G8_SSCALED: return "VK_FORMAT_R8G8_SSCALED";
    case VK_FORMAT_B8G8R8A8_SRGB: return "VK_FORMAT_B8G8R8A8_SRGB";
    case VK_FORMAT_B8G8R8A8_UNORM: return "VK_FORMAT_B8G8R8A8_UNORM";
    case VK_FORMAT_R8G8B8_UNORM: return "VK_FORMAT_R8G8B8_UNORM";
    case VK_FORMAT_B8G8R8_UNORM: return "VK_FORMAT_B8G8R8_UNORM";
    case VK_FORMAT_R32G32_SFLOAT: return "VK_FORMAT_R32G32_SFLOAT";
    default:
        LOG_INFO("[vk] unhandled format: %d", format);
        return "unknown";
    }

    return "unknown";
}

extern const char* sz_from_enum(VkPresentModeKHR mode) INTERNAL
{
    switch (mode) {
    case VK_PRESENT_MODE_IMMEDIATE_KHR: return "VK_PRESENT_MODE_IMMEDIATE_KHR";
    case VK_PRESENT_MODE_MAILBOX_KHR: return "VK_PRESENT_MODE_MAILBOX_KHR";
    case VK_PRESENT_MODE_FIFO_KHR: return "VK_PRESENT_MODE_FIFO_KHR";
    case VK_PRESENT_MODE_FIFO_RELAXED_KHR: return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
    case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR: return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
    case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
    case VK_PRESENT_MODE_MAX_ENUM_KHR: return "VK_PRESENT_MODE_MAX_ENUM_KHR";
    }
    return "unknown";
}

extern u32 hash32(const GfxVkBuffer &it, u32 seed /*= MURMUR3_SEED*/) INTERNAL
{
    u32 state = seed;
    state = hash32(it.handle, state);
    state = hash32(it.size, state);
    return state;
}

extern u32 hash32(const GfxVkTexture &it, u32 seed /*= MURMUR3_SEED*/) INTERNAL
{
    u32 state = seed;
    state = hash32(it.image, state);
    state = hash32(it.view, state);
    return state;
}

extern u32 hash32(const GfxVkDescriptorDesc &desc, u32 seed /*= MURMUR3_SEED*/) INTERNAL
{
    u32 state = seed;
    state = hash32(desc.binding, state);
    state = hash32(desc.type, state);

    switch (desc.type) {
    case GFX_TEXTURE:
        state = hash32(desc.texture.texture, state);
        state = hash32(desc.texture.sampler, state);
        break;
    case GFX_TEXTURE_ARRAY:
        state = hash32(desc.texture_array.count, state);
        for (i32 i = 0; i < desc.texture_array.count; i++) {
            state = hash32(desc.texture_array.textures[i], state);
            state = hash32(desc.texture_array.samplers[i], state);
        }
        break;
    case GFX_UNIFORM:
        state = hash32(desc.uniform, state);
        break;
    }

    return state;
}

extern u32 hash32(const GfxVkDescriptorSetDesc &desc, u32 seed /*=MURMUR3_SEED*/) INTERNAL
{
    u32 state = seed;
    state = hash32(desc.pipeline_layout, state);
    state = hash32(desc.set_layout, state);
    for (auto it : desc.bindings) state = hash32(it, state);
    return state;
}

extern u32 hash32(const VkDescriptorSetLayoutCreateInfo &info, u32 seed /*=MURMUR3_SEED*/) INTERNAL
{
    u32 state = seed;
    PANIC_IF(info.pNext, "unimplemented path; need to go through the chain of vulkan structures here and hash it all");

    state = hash32(info.sType, state);
    state = hash32(info.flags, state);

    for (u32 i = 0; i < info.bindingCount; i++) {
        auto &binding = info.pBindings[i];

        state = hash32(binding.binding, state);
        state = hash32(binding.descriptorType, state);
        state = hash32(binding.descriptorCount, state);
        state = hash32(binding.stageFlags, state);

        PANIC_IF(binding.pImmutableSamplers, "unimplemented path, double check that pImmuteableSamplers is an array of binding.descriptorCount length, and add it to hash");
    }


    return state;
}
