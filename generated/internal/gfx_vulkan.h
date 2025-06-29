#ifndef GFX_VULKAN_INTERNAL_H
#define GFX_VULKAN_INTERNAL_H

extern bool vk_image_format_supported(VkFormat format, VkImageCreateInfo image_info);
extern void vk_imm_begin();
extern void vk_imm_end();
extern void vk_destroy_swapchain();
extern void vk_create_swapchain(VkExtent2D extent);
extern void vk_copy_buffer(VkCommandBuffer cmd, VkBuffer dst, VkBuffer src, VkDeviceSize size);
extern void vk_copy_buffer_to_image(VkCommandBuffer cmd, VkBuffer buffer, VkImage image, u32 width, u32 height, i32 dst_x = 0, i32 dst_y = 0);
extern GfxVkTexture vk_create_texture(VkFormat format, VkComponentMapping swizzle, u32 width, u32 height, VkImageUsageFlags usage);
extern GfxVkTexture vk_create_texture(VkFormat format, VkComponentMapping swizzle, u32 width, u32 height);
extern GfxVkTexture vk_create_texture(void *pixels, VkFormat format, VkComponentMapping swizzle, u32 width, u32 height);
extern GfxVkTexture vk_create_depth_texture(VkExtent2D extent);
extern void vk_set_viewport(VkCommandBuffer cmd, f32 width, f32 height);
extern void vk_set_scissor(VkCommandBuffer cmd, f32 width, f32 height);
extern void vk_transition_image(VkCommandBuffer cmd, GfxVkTexture texture, VkImageLayout old_layout, VkImageLayout new_layout);
extern void vk_transition_image(VkCommandBuffer cmd, VkImage image, VkImageAspectFlags aspect, VkImageLayout old_layout, VkImageLayout new_layout);
extern VkDeviceAddress vk_get_buffer_address(VkBuffer buffer);
extern GfxVkBuffer vk_create_buffer(i32 size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage, VmaAllocationCreateFlagBits flags);
extern GfxVkBuffer vk_create_buffer(void *data, i32 size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage, VmaAllocationCreateFlagBits flags);
extern void vk_destroy_buffer(GfxVkBuffer buffer);
extern void vk_set_image_label(VkImage image, String label);
extern void vk_set_image_view_label(VkImageView view, String label);
extern void vk_set_texture_label(GfxVkTexture texture, String label);
extern void vk_set_buffer_label(VkBuffer buffer, String label);
extern void vk_set_pipeline_label(GfxPipeline handle, String label);
extern VkImageAspectFlags vk_aspect_mask(VkFormat format);
extern VkFormat vk_format(GfxTextureFormat format);
extern u32 vk_format_size(VkFormat format);
extern u32 vk_block_size(VkFormat format);
extern VkSamplerAddressMode vk_wrap_mode(GfxSampleWrap wrap);
extern VkBorderColor vk_border_color(GfxSampleBorderColor border_color);
extern VkFilter vk_filter(GfxSampleFilter filter);
extern VkComponentMapping vk_component_mapping(GfxSwizzle swizzle);
extern const char *sz_from_enum(shaderc_compilation_status status);
extern const char *sz_from_enum(VkResult result);
extern const char *sz_from_enum(VkFormat format);
extern const char *sz_from_enum(VkPresentModeKHR mode);
extern u32 hash32(const GfxTextureAssetDesc & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkBuffer & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkTexture & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkDescriptorDesc & desc, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkDescriptorSetDesc & desc, u32 seed =MURMUR3_SEED);
extern u32 hash32(const VkDescriptorSetLayoutCreateInfo & info, u32 seed =MURMUR3_SEED);
static VkAttachmentLoadOp vk_load_op(GfxLoadOp op);
static VkAttachmentStoreOp vk_store_op(GfxStoreOp op);

#endif // GFX_VULKAN_INTERNAL_H
