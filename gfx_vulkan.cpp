#define GFX_VULKAN_INTERNAL
#include "gfx_vulkan.h"
#include "gfx.h"
#include "string.h"
#include "memory.h"

#include "vulkan/vulkan_core.h"

GfxVkContext vk;

extern bool vk_image_format_supported(
    VkFormat format,
    VkImageCreateInfo image_info) INTERNAL
{
    VkFormatFeatureFlags required_flags = 0;
    if (image_info.usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        required_flags |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
    if (image_info.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        required_flags |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
    if (image_info.usage & VK_IMAGE_USAGE_SAMPLED_BIT)
        required_flags |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    if (image_info.usage & VK_IMAGE_USAGE_STORAGE_BIT)
        required_flags |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    if (image_info.usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
        required_flags |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    if (image_info.usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        required_flags |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(vk.physical_device.handle, format, &props);

    VkFormatFeatureFlags supported_flags;
    if (image_info.tiling == VK_IMAGE_TILING_LINEAR) {
        supported_flags = props.linearTilingFeatures;
    } else if (image_info.tiling == VK_IMAGE_TILING_OPTIMAL) {
        supported_flags = props.optimalTilingFeatures;
    } else {
        LOG_ERROR("[gfx] unknown tiling mode: %d", image_info.tiling);
        return false;
    }

    if ((supported_flags & required_flags) != required_flags) return false;

    VkImageFormatProperties format_properties;
    VkResult result = vkGetPhysicalDeviceImageFormatProperties(
        vk.physical_device.handle,
        format,
        image_info.imageType,
        image_info.tiling,
        image_info.usage,
        image_info.flags,
        &format_properties);

    if (result != VK_SUCCESS) {
        LOG_ERROR(
            "[gfx] unable to retrieve image format properties for format %d and { imageType: %d, tiling: %d, usage: %d, flags: %d }",
            format,
            image_info.imageType, image_info.tiling, image_info.usage, image_info.flags);
        return false;
    }

    if (image_info.extent.width > format_properties.maxExtent.width ||
        image_info.extent.height > format_properties.maxExtent.height ||
        image_info.extent.depth > format_properties.maxExtent.depth)
    {
        return false;
    }

    if (image_info.mipLevels > format_properties.maxMipLevels) return false;
    if (image_info.arrayLayers > format_properties.maxArrayLayers) return false;
    if (!(format_properties.sampleCounts & image_info.samples)) return false;
    return true;
}


extern void vk_imm_begin() INTERNAL
{
    VK_CHECK(vkResetFences(vk.device, 1, &vk.imm.fence));
    VK_CHECK(vkResetCommandBuffer(vk.imm.cmd, 0));

    VkCommandBufferBeginInfo begin_info{
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    VK_CHECK(vkBeginCommandBuffer(vk.imm.cmd, &begin_info));
}

extern void vk_imm_end() INTERNAL
{
    VK_CHECK(vkEndCommandBuffer(vk.imm.cmd));

    VkCommandBufferSubmitInfo cmd_info{
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = vk.imm.cmd,
    };

    VkSubmitInfo2 submit_info{
        VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .commandBufferInfoCount = 1, .pCommandBufferInfos = &cmd_info,
    };
    VK_CHECK(vkQueueSubmit2(vk.queues.graphics, 1, &submit_info, vk.imm.fence));
    vkWaitForFences(vk.device, 1, &vk.imm.fence, VK_TRUE, UINT64_MAX);
}


Vector2 gfx_resolution() EXPORT
{
    return { (f32)vk.swapchain.extent.width, (f32)vk.swapchain.extent.height };
}

void gfx_wait_frame() EXPORT
{
    auto *frame = &vk.frames[vk.current_frame];
    VK_CHECK(vkWaitForFences(vk.device, 1, &frame->fence, VK_TRUE, UINT64_MAX));
}

extern void vk_destroy_swapchain() INTERNAL
{
    for (auto it : vk.swapchain.views) 
        vkDestroyImageView(vk.device, it, nullptr);
    vk.swapchain.views.count = 0;

    if (vk.swapchain.handle) {
        vkDestroySwapchainKHR(vk.device, vk.swapchain.handle, nullptr);
        vk.swapchain.handle = VK_NULL_HANDLE;
    }
}

extern void vk_create_swapchain(VkExtent2D extent) INTERNAL
{
    LOG_INFO("[vk] creating swapchain: %d, %d", extent.width, extent.height);
    SArena scratch = tl_scratch_arena();

    VkSurfaceCapabilitiesKHR surface_caps{};
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk.physical_device.handle, vk.surface, &surface_caps));

    vk.swapchain.extent.width = CLAMP(
        extent.width,
        surface_caps.minImageExtent.width,
        surface_caps.maxImageExtent.width);

    vk.swapchain.extent.height = CLAMP(
        extent.height,
        surface_caps.minImageExtent.height,
        surface_caps.maxImageExtent.height);

    u32 format_count = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(vk.physical_device.handle, vk.surface, &format_count, nullptr));
    auto formats = array_create<VkSurfaceFormatKHR>(format_count, scratch);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(vk.physical_device.handle, vk.surface, &format_count, formats.data));

    i32 chosen_format = -1;
    for (auto it : iterator(formats)) {
        LOG_INFO("swapchain [%d]: %s", it.index, sz_from_enum(it->format));
        if (it->format == VK_FORMAT_B8G8R8A8_SRGB &&
            it->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            chosen_format = it.index;
        }
    }
    PANIC_IF(chosen_format == -1, "no suitable surface format found");
    LOG_INFO("chosen swapchain format: %s [%u]", sz_from_enum(formats[chosen_format].format), formats[chosen_format].format);
    vk.swapchain.format = formats[chosen_format].format;

    u32 present_modes = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(vk.physical_device.handle, vk.surface, &present_modes, nullptr));
    auto modes = array_create<VkPresentModeKHR>(present_modes, scratch);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(vk.physical_device.handle, vk.surface, &present_modes, modes.data));

    i32 chosen_mode = -1;
    for (auto it : iterator(modes)) {
        LOG_INFO("present mode [%d]: %s", it.index, sz_from_enum(*it));
        if (it == VK_PRESENT_MODE_FIFO_KHR) chosen_mode = it.index;
    }
    PANIC_IF(chosen_mode == -1, "no suitable present mode found");
    LOG_INFO("chosen present mode: %s", sz_from_enum(modes[chosen_mode]));
    
    PANIC_IF(surface_caps.minImageCount > MAX_SWAPCHAIN_IMAGES,
             "[vk] minimum image count surface capability higher than supported number of swapchain images");

    VkSwapchainCreateInfoKHR swapchain_info{
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface            = vk.surface,
        .minImageCount      = MIN(surface_caps.minImageCount, MAX_SWAPCHAIN_IMAGES),
        .imageFormat        = formats[chosen_format].format,
        .imageColorSpace    = formats[chosen_format].colorSpace,
        .imageExtent        = vk.swapchain.extent,
        .imageArrayLayers   = 1,
        .imageUsage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform       = surface_caps.currentTransform,
        .compositeAlpha     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode        = modes[chosen_mode],
        .clipped            = VK_TRUE,
    };

    u32 queue_indices[] = {
        vk.physical_device.graphics_family_index,
        vk.physical_device.present_family_index
    };

    if (vk.physical_device.graphics_family_index != vk.physical_device.present_family_index) {
        swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_info.queueFamilyIndexCount = ARRAY_COUNT(queue_indices);
        swapchain_info.pQueueFamilyIndices = queue_indices;
    }

    VK_CHECK(vkCreateSwapchainKHR(vk.device, &swapchain_info, nullptr, &vk.swapchain.handle));

    u32 image_count = 0;
    VK_CHECK(vkGetSwapchainImagesKHR(
            vk.device, vk.swapchain.handle, 
            &image_count, nullptr));

    vk.swapchain.images.count = image_count;
    vk.swapchain.views.count = image_count;
    vk.swapchain.render_finished.count = image_count;

    vkGetSwapchainImagesKHR(
        vk.device, vk.swapchain.handle, 
        &image_count, vk.swapchain.images.data);

    for (auto it : iterator(vk.swapchain.views)) {
        VkImageViewCreateInfo view_info{
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image            = vk.swapchain.images[it.index],
            .viewType         = VK_IMAGE_VIEW_TYPE_2D,
            .format           = formats[chosen_format].format,
            .subresourceRange = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
        };

        VK_CHECK(vkCreateImageView(vk.device, &view_info, nullptr, &it.elem()));

        VkSemaphoreCreateInfo semaphore_info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        VK_CHECK(vkCreateSemaphore(vk.device, &semaphore_info, nullptr, &vk.swapchain.render_finished[it.index]));
    }

    vk.swapchain.depth = vk_create_depth_texture(vk.swapchain.extent);
}

void gfx_wait_for_frame() EXPORT
{
    vk.current_frame = (vk.current_frame + 1) % vk.frames.count;

    auto *frame = &vk.frames[vk.current_frame];
    frame->vertices.offset = 0;

    VK_CHECK(vkWaitForFences(vk.device, 1, &frame->fence, VK_TRUE, UINT64_MAX));

}

extern void vk_recreate_swapchain() INTERNAL
{
    vkDeviceWaitIdle(vk.device);

    vk_destroy_swapchain();
    for (auto& frame : vk.frames) {
        vkDestroySemaphore(vk.device, frame.image_available, nullptr);
        frame.image_available = nullptr;
    }

    vk_create_swapchain(vk.swapchain.extent);
    for (auto &frame : vk.frames) {
        VkSemaphoreCreateInfo semaphore_info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        VK_CHECK(vkCreateSemaphore(vk.device, &semaphore_info, nullptr, &frame.image_available));
    }

    vk.recreate_swapchain_requested = false;
}

extern void vk_copy_buffer(VkCommandBuffer cmd, VkBuffer dst, VkBuffer src, VkDeviceSize size) INTERNAL
{
    VkBufferCopy copy{ .size = size, };
    vkCmdCopyBuffer(cmd, src, dst, 1, &copy);
}

extern void vk_copy_buffer_to_image(
    VkCommandBuffer cmd,
    VkBuffer buffer, VkImage image,
    u32 width, u32 height,
    i32 dst_x/*= 0*/, i32 dst_y /*= 0*/) INTERNAL
{
    VkBufferImageCopy copy{
        .bufferOffset      = 0,
        .bufferRowLength   = 0,
        .bufferImageHeight = 0,
        .imageSubresource  = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
        .imageOffset = { dst_x, dst_y, 0 },
        .imageExtent = { width, height, 1 },
    };

    vkCmdCopyBufferToImage(cmd, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
}


GfxBuffer gfx_create_buffer(i32 size) EXPORT
{
    GfxVkBuffer buffer = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    return (GfxBuffer)array_add(&vk.buffers, buffer);
}

GfxBuffer gfx_create_buffer(void *data, i32 size) EXPORT
{
    GfxVkBuffer buffer = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    GfxVkBuffer staging = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    vmaMapMemory(vk.allocator, staging.allocation, &staging.allocation_info.pMappedData);
    memcpy(staging.allocation_info.pMappedData, data, size);
    vmaUnmapMemory(vk.allocator, staging.allocation);

    VK_IMM vk_copy_buffer(vk.imm.cmd, buffer, staging, size);
    vk_destroy_buffer(staging);
    return (GfxBuffer)array_add(&vk.buffers, buffer);
}

GfxBuffer gfx_create_vertex_buffer(void *data, i32 size) EXPORT
{
    GfxVkBuffer buffer = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    GfxVkBuffer staging = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    vmaMapMemory(vk.allocator, staging.allocation, &staging.allocation_info.pMappedData);
    memcpy(staging.allocation_info.pMappedData, data, size);
    vmaUnmapMemory(vk.allocator, staging.allocation);

    VK_IMM vk_copy_buffer(vk.imm.cmd, buffer, staging, size);
    vk_destroy_buffer(staging);
    return (GfxBuffer)array_add(&vk.buffers, buffer);
}

GfxBuffer gfx_create_index_buffer(void *data, i32 size) EXPORT
{
    GfxVkBuffer buffer = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    GfxVkBuffer staging = vk_create_buffer(
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    vmaMapMemory(vk.allocator, staging.allocation, &staging.allocation_info.pMappedData);
    memcpy(staging.allocation_info.pMappedData, data, size);
    vmaUnmapMemory(vk.allocator, staging.allocation);

    VK_IMM vk_copy_buffer(vk.imm.cmd, buffer, staging, size);
    vk_destroy_buffer(staging);
    return (GfxBuffer)array_add(&vk.buffers, buffer);
}

GfxTexture gfx_load_texture(String path, bool sRGB /*= true*/) EXPORT
{
    AssetHandle handle = find_asset_handle(path);
    return gfx_load_texture(handle, sRGB);
}

GfxTexture gfx_load_texture(AssetHandle handle, bool sRGB /*= true*/) EXPORT
{
    if (!handle) return GfxTexture_INVALID;

    if (auto *asset = map_find(&vk.texture_assets, { handle, sRGB })) {
        return *asset;
    }

    GfxTextureAsset *asset = get_asset<GfxTextureAsset>(handle);
    if (!asset) return GfxTexture_INVALID;

    GfxSwizzle swizzle = GFX_SWIZZLE_IDENTITY;

    GfxTextureFormat format = sRGB ?
        gfx_format_srgb(asset->format) :
        gfx_format_unorm(asset->format);

    GfxVkTexture texture = vk_create_texture(
        asset->data,
        vk_format(format),
        vk_component_mapping(swizzle),
        asset->width, asset->height);

    vk_set_texture_label(texture, get_asset_identifier(handle));

    i32 idx = array_add(&vk.textures, texture);
    GfxTexture texture_handle = GfxTexture(idx);

    map_set(&vk.texture_assets, { handle, sRGB }, texture_handle);
    return texture_handle;
}

extern GfxVkTexture vk_create_texture(
    VkFormat format,
    VkComponentMapping swizzle,
    u32 width, u32 height,
    VkImageUsageFlags usage) INTERNAL
{
    GfxVkTexture texture{ .format = format, .width = width, .height = height };

    VkImageCreateInfo image_info{
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType   = VK_IMAGE_TYPE_2D,
        .format      = format,
        .extent      = { width, height, 1 },
        .mipLevels   = 1,
        .arrayLayers = 1,
        .samples     = VK_SAMPLE_COUNT_1_BIT,
        .tiling      = VK_IMAGE_TILING_OPTIMAL,
        .usage       = usage,
    };

    if (!vk_image_format_supported(image_info.format, image_info)) {
        switch (image_info.format) {
        case VK_FORMAT_R8G8B8_UNORM:
            if (vk_image_format_supported(VK_FORMAT_B8G8R8_UNORM, image_info)) {
                image_info.format = VK_FORMAT_B8G8R8A8_UNORM;
                swizzle = { VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_R };
                LOG_INFO("[vulkan] using B8G8R8A8_UNORM instead of R8G8B8_UNORM, swizzle adjusted");
            } else if (vk_image_format_supported(VK_FORMAT_R8G8B8A8_UNORM, image_info)) {
                image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
            } else {
                LOG_ERROR("[vulkan] unsupported texture format: %s", sz_from_enum(image_info.format));
                return {};
            }
            break;
        default:
            LOG_ERROR("[vulkan] unsupported texture format: %s", sz_from_enum(image_info.format));
            return {};
        }
    }

    VmaAllocationCreateInfo alloc_info{
        .usage         = VMA_MEMORY_USAGE_GPU_ONLY,
        .requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    };
    VK_CHECK(vmaCreateImage(vk.allocator, &image_info, &alloc_info, &texture.image, &texture.allocation, &texture.allocation_info));

    VkImageViewCreateInfo view_info{
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image    = texture.image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format   = image_info.format,
        .components = swizzle,
        .subresourceRange = {
            .aspectMask     = vk_aspect_mask(format),
            .baseMipLevel   = 0, .levelCount = image_info.mipLevels,
            .baseArrayLayer = 0, .layerCount = image_info.arrayLayers,
        },
    };

    VK_CHECK(vkCreateImageView(vk.device, &view_info, nullptr, &texture.view));
    return texture;
}

extern GfxVkTexture vk_create_texture(
    VkFormat format,
    VkComponentMapping swizzle,
    u32 width, u32 height) INTERNAL
{
    GfxVkTexture texture = vk_create_texture(
        format, swizzle,
        width, height,
        VK_IMAGE_USAGE_SAMPLED_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT);

    VK_IMM vk_transition_image(vk.imm.cmd, texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    return texture;
}

extern GfxVkTexture vk_create_texture(
    void *pixels,
    VkFormat format,
    VkComponentMapping swizzle,
    u32 width, u32 height) INTERNAL
{
    GfxVkTexture texture = vk_create_texture(format, swizzle, width, height);
    if (!texture) return {};

    u32 block_size = vk_block_size(format);
    auto staging = vk_create_buffer(
        width*height*block_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    if (!staging) {
        LOG_ERROR("[gfx][vk] unable to create staging buffer for texture");
        return {};
    }


    vmaMapMemory(vk.allocator, staging.allocation, &staging.allocation_info.pMappedData);
    memcpy(staging.allocation_info.pMappedData, pixels, width*height*block_size);
    vmaUnmapMemory(vk.allocator, staging.allocation);

    VK_IMM {
        vk_transition_image(vk.imm.cmd, texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        vk_copy_buffer_to_image(vk.imm.cmd, staging, texture.image, width, height);
        vk_transition_image(vk.imm.cmd, texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    vk_destroy_buffer(staging);
    return texture;
}

extern GfxVkTexture vk_create_depth_texture(VkExtent2D extent) INTERNAL
{
    return vk_create_texture(
        VK_FORMAT_D16_UNORM,
        vk_component_mapping(GFX_SWIZZLE_IDENTITY),
        extent.width, extent.height,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

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

extern VkDeviceAddress vk_get_buffer_address(VkBuffer buffer) INTERNAL
{
    VkBufferDeviceAddressInfo address_info{
        VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = buffer,
    };
    return vkGetBufferDeviceAddress(vk.device, &address_info);
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

extern void vk_set_image_label(VkImage image, String label) INTERNAL
{
    if (!vkSetDebugUtilsObjectNameEXT) return;

    VkDebugUtilsObjectNameInfoEXT name_info{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .objectType = VK_OBJECT_TYPE_IMAGE,
        .objectHandle = (u64)image,
        .pObjectName = sz_stringf(mem_dynamic, "image/%.*s [%llX]", STRFMT(label), image),
    };

    vkSetDebugUtilsObjectNameEXT(vk.device, &name_info);
}

extern void vk_set_image_view_label(VkImageView view, String label) INTERNAL
{
    if (!vkSetDebugUtilsObjectNameEXT) return;
    SArena scratch = tl_scratch_arena();

    VkDebugUtilsObjectNameInfoEXT name_info{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .objectType = VK_OBJECT_TYPE_IMAGE_VIEW,
        .objectHandle = (u64)view,
        .pObjectName = sz_stringf(scratch, "image_view/%.*s [%llX]", STRFMT(label), view),
    };

    vkSetDebugUtilsObjectNameEXT(vk.device, &name_info);
}

extern void vk_set_texture_label(GfxVkTexture texture, String label) INTERNAL
{
    vk_set_image_label(texture.image, label);
    vk_set_image_view_label(texture.view, label);
}

extern void vk_set_buffer_label(VkBuffer buffer, String label) INTERNAL
{
    if (!vkSetDebugUtilsObjectNameEXT) return;
    SArena scratch = tl_scratch_arena();

    VkDebugUtilsObjectNameInfoEXT name_info{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .objectType = VK_OBJECT_TYPE_BUFFER,
        .objectHandle = (u64)buffer,
        .pObjectName = sz_stringf(scratch, "buffer/%.*s [%llX]", STRFMT(label), buffer),
    };

    vkSetDebugUtilsObjectNameEXT(vk.device, &name_info);
}

extern void vk_set_pipeline_label(GfxPipeline handle, String label) INTERNAL
{
    if (!vkSetDebugUtilsObjectNameEXT) return;
    SArena scratch = tl_scratch_arena();

    VkPipeline pipeline = vk.pipelines[handle];

    VkDebugUtilsObjectNameInfoEXT name_info{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .objectType = VK_OBJECT_TYPE_PIPELINE,
        .objectHandle = (u64)pipeline,
        .pObjectName = sz_stringf(scratch, "pipeline/%.*s [%llX]", STRFMT(label), handle),
    };

    vkSetDebugUtilsObjectNameEXT(vk.device, &name_info);
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
    case GFX_TEXTURE_R8_UNORM:       return VK_FORMAT_R8_UNORM;
    case GFX_TEXTURE_R8_SRGB:        return VK_FORMAT_R8_SRGB;
    case GFX_TEXTURE_R8G8_UNORM:     return VK_FORMAT_R8G8_UNORM;
    case GFX_TEXTURE_R8G8_SRGB:      return VK_FORMAT_R8G8_SRGB;
    case GFX_TEXTURE_R8G8B8_UNORM:   return VK_FORMAT_R8G8B8_UNORM;
    case GFX_TEXTURE_R8G8B8_SRGB:    return VK_FORMAT_R8G8B8_SRGB;
    case GFX_TEXTURE_R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
    case GFX_TEXTURE_R8G8B8A8_SRGB:  return VK_FORMAT_R8G8B8A8_SRGB;
    }

    PANIC("[gfx] unknown texture format: %s [%d]", sz_from_enum(format), format);
    return VK_FORMAT_UNDEFINED;
}

extern u32 vk_format_size(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_SRGB:
        return 1;
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8_SRGB:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
        return 2;
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8_SRGB:
    case VK_FORMAT_B8G8R8_UNORM:
    case VK_FORMAT_B8G8R8_SNORM:
    case VK_FORMAT_B8G8R8_UINT:
    case VK_FORMAT_B8G8R8_SINT:
    case VK_FORMAT_B8G8R8_SRGB:
        return 3;
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_SNORM:
    case VK_FORMAT_B8G8R8A8_UINT:
    case VK_FORMAT_B8G8R8A8_SINT:
    case VK_FORMAT_B8G8R8A8_SRGB:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_SFLOAT:
        return 4;
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16_SFLOAT:
        return 6;
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_SFLOAT:
        return 8;
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32_SFLOAT:
        return 12;
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return 16;
    case VK_FORMAT_D16_UNORM:
        return 2;
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return 4;
    case VK_FORMAT_D32_SFLOAT:
        return 4;
    case VK_FORMAT_S8_UINT:
        return 1;
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return 3;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return 5;
    case VK_FORMAT_R4G4_UNORM_PACK8:
        return 1;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        return 2;
    default:
        PANIC("[gfx] unhandled VkFormat size '%s' [%d]", sz_from_enum(format), format);
        return 0;
    }
}

extern u32 vk_block_size(VkFormat format) INTERNAL
{
    switch (format) {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_SRGB:
        return 1;
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8_SRGB:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
        return 2;
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8_SRGB:
    case VK_FORMAT_B8G8R8_UNORM:
    case VK_FORMAT_B8G8R8_SNORM:
    case VK_FORMAT_B8G8R8_UINT:
    case VK_FORMAT_B8G8R8_SINT:
    case VK_FORMAT_B8G8R8_SRGB:
        return 3;
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_SNORM:
    case VK_FORMAT_B8G8R8A8_UINT:
    case VK_FORMAT_B8G8R8A8_SINT:
    case VK_FORMAT_B8G8R8A8_SRGB:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_SFLOAT:
        return 4;
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16_SFLOAT:
        return 6;
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_SFLOAT:
        return 8;
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32_SFLOAT:
        return 12;
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return 16;
    case VK_FORMAT_D16_UNORM:
        return 2;
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return 4;
    case VK_FORMAT_D32_SFLOAT:
        return 4;
    case VK_FORMAT_S8_UINT:
        return 1;
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return 3;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return 5;
    case VK_FORMAT_R4G4_UNORM_PACK8:
        return 1;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        return 2;
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
    case VK_FORMAT_BC4_UNORM_BLOCK:
    case VK_FORMAT_BC4_SNORM_BLOCK:
        return 8;
    case VK_FORMAT_BC2_UNORM_BLOCK:
    case VK_FORMAT_BC2_SRGB_BLOCK:
    case VK_FORMAT_BC3_UNORM_BLOCK:
    case VK_FORMAT_BC3_SRGB_BLOCK:
    case VK_FORMAT_BC5_UNORM_BLOCK:
    case VK_FORMAT_BC5_SNORM_BLOCK:
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
    case VK_FORMAT_BC7_UNORM_BLOCK:
    case VK_FORMAT_BC7_SRGB_BLOCK:
        return 16;
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        return 8;
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        return 16;
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
        return 16;
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
        return 8;
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return 16;
    default:
        PANIC("[gfx] unsupported format: %s", sz_from_enum(format));
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

    LOG_ERROR("[gfx] unknown shaderc compilation status: %d", status);
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

    LOG_ERROR("[gfx] unknown vulkan result: %d", result);
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
    case VK_FORMAT_R8G8B8A8_UNORM: return "VK_FORMAT_R8G8B8A8_UNORM";
    case VK_FORMAT_R8G8B8A8_SRGB: return "VK_FORMAT_R8G8B8A8_SRGB";
    default:
        LOG_ERROR("[vk] unhandled format: %d", format);
        return "unknown";
    }

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

    LOG_ERROR("[gfx] unknown present mode: %d", mode);
    return "unknown";
}

void gfx_begin_pass(const GfxVkRenderPassDesc& desc) EXPORT
{
    SArena scratch = tl_scratch_arena();

    auto *frame = &vk.frames[vk.current_frame];
    auto cmd = frame->cmd;

    i32 color_count;
    VkRenderingAttachmentInfo color_attachments[MAX_COLOR_ATTACHMENTS];
    for (color_count = 0; color_count < MAX_COLOR_ATTACHMENTS; color_count++) {
        if (!desc.color[color_count].res) break;

        color_attachments[color_count] = VkRenderingAttachmentInfo {
            VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView   = desc.color[color_count].res.view,
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp      = vk_load_op(desc.color[color_count].load_op),
            .storeOp     = vk_store_op(desc.color[color_count].store_op),
            .clearValue.color.float32 = {
                desc.color[color_count].clear[0],
                desc.color[color_count].clear[1],
                desc.color[color_count].clear[2],
                desc.color[color_count].clear[3],
            }
        };
    }

    VkRenderingAttachmentInfo depth_attachment {
        VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
    };

    if (desc.depth.res) {
        depth_attachment.imageView   = desc.depth.res.view;
        depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depth_attachment.loadOp      = vk_load_op(desc.depth.load_op);
        depth_attachment.storeOp     = vk_store_op(desc.depth.store_op);
        depth_attachment.clearValue.depthStencil = {
            desc.depth.clear[0],
            u32(desc.depth.clear[1]),
        };
    };

    VkRenderingInfo render_info{
        VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {
            .extent = { desc.extent.w, desc.extent.h }
        },
        .layerCount = 1,
        .colorAttachmentCount = u32(color_count),
        .pColorAttachments = &color_attachments[0],
        .pDepthAttachment = desc.depth.res ? &depth_attachment : nullptr
    };



    if (vkCmdBeginDebugUtilsLabelEXT) {
        VkDebugUtilsLabelEXT debug_label {
            VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = desc.debug_name ? sz_string(desc.debug_name, scratch) : "",
            .color = { desc.debug_color[0], desc.debug_color[1], desc.debug_color[2], desc.debug_color[3] }
        };

        vkCmdBeginDebugUtilsLabelEXT(cmd, &debug_label);
    }

    vkCmdBeginRendering(cmd, &render_info);
}

void gfx_end_pass() EXPORT
{
    auto *frame = &vk.frames[vk.current_frame];
    auto cmd = frame->cmd;

    vkCmdEndRendering(cmd);
    if (vkCmdEndDebugUtilsLabelEXT) vkCmdEndDebugUtilsLabelEXT(cmd);
}

extern void vk_update_uniform_buffer(GfxVkBuffer buffer, void *data, i32 size) INTERNAL
{
    vmaMapMemory(vk.allocator, buffer.allocation, &buffer.allocation_info.pMappedData);
    memcpy(buffer.allocation_info.pMappedData, data, size);
    vmaUnmapMemory(vk.allocator, buffer.allocation);
}

VkAttachmentLoadOp vk_load_op(GfxLoadOp op) INTERNAL
{
    switch (op) {
    case GFX_LOAD_OP_DONT_CARE: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    case GFX_LOAD_OP_LOAD: return VK_ATTACHMENT_LOAD_OP_LOAD;
    case GFX_LOAD_OP_CLEAR: return VK_ATTACHMENT_LOAD_OP_CLEAR;
    }

    LOG_ERROR("[gfx] unknown attachment load op: %d", op);
    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
}

VkAttachmentStoreOp vk_store_op(GfxStoreOp op) INTERNAL
{
    switch (op) {
    case GFX_STORE_OP_DONT_CARE: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    case GFX_STORE_OP_STORE: return VK_ATTACHMENT_STORE_OP_STORE;
    }

    LOG_ERROR("[gfx] unknown attachment store op: %d", op);
    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
}

const char* sz_from_enum(GfxLoadOp op) EXPORT
{
    switch (op) {
    case GFX_LOAD_OP_DONT_CARE: return "GFX_LOAD_OP_DONT_CARE";
    case GFX_LOAD_OP_LOAD: return "GFX_LOAD_OP_LOAD";
    case GFX_LOAD_OP_CLEAR: return "GFX_LOAD_OP_CLEAR";
    }

    LOG_ERROR("[gfx] unknown load op: %d", op);
    return "unknown";
}

const char* sz_from_enum(GfxStoreOp op) EXPORT
{
    switch (op) {
    case GFX_STORE_OP_DONT_CARE: return "GFX_STORE_OP_DONT_CARE";
    case GFX_STORE_OP_STORE: return "GFX_STORE_OP_STORE";
    }

    LOG_ERROR("[gfx] unknown store op: %d", op);
    return "unknown";
}

extern bool operator==(const VkExtensionProperties &lhs, const char *rhs) INTERNAL
{
    return strcmp(lhs.extensionName, rhs) == 0;
}

extern bool operator==(const VkLayerProperties &lhs, const char *rhs) INTERNAL
{
    return strcmp(lhs.layerName, rhs) == 0;
}

extern bool operator==(
    const VkDescriptorSetLayoutCreateInfo &lhs,
    const VkDescriptorSetLayoutCreateInfo &rhs) INTERNAL
{
    if (lhs.sType != rhs.sType ||
        lhs.flags != rhs.flags ||
        lhs.bindingCount != rhs.bindingCount)
    {
        return false;
    }

    if (lhs.pNext != rhs.pNext) {
        PANIC("unimplemented vulkan structure chain comparison");
        return false;
    }

    for (u32 i = 0; i < lhs.bindingCount; i++) {
        if (lhs.pBindings[i].binding != rhs.pBindings[i].binding ||
            lhs.pBindings[i].descriptorType != rhs.pBindings[i].descriptorType ||
            lhs.pBindings[i].descriptorCount != rhs.pBindings[i].descriptorCount ||
            lhs.pBindings[i].stageFlags != rhs.pBindings[i].stageFlags)
        {
            return false;
        }

        if (lhs.pBindings[i].pImmutableSamplers != rhs.pBindings[i].pImmutableSamplers)
        {
            PANIC("unimplemented immutable sampler comparison");
            return false;
        }
    }

    return true;
}
