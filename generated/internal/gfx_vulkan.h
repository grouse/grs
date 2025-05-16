#ifndef GFX_VULKAN_INTERNAL_H
#define GFX_VULKAN_INTERNAL_H

extern void vk_set_viewport(VkCommandBuffer cmd, f32 width, f32 height);
extern void vk_set_scissor(VkCommandBuffer cmd, f32 width, f32 height);
extern void vk_transition_image(VkCommandBuffer cmd, GfxVkTexture texture, VkImageLayout old_layout, VkImageLayout new_layout);
extern void vk_transition_image(VkCommandBuffer cmd, VkImage image, VkImageAspectFlags aspect, VkImageLayout old_layout, VkImageLayout new_layout);
extern GfxVkBuffer vk_create_buffer(i32 size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage, VmaAllocationCreateFlagBits flags);
extern GfxVkBuffer vk_create_buffer(void *data, i32 size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage, VmaAllocationCreateFlagBits flags);
extern void vk_destroy_buffer(GfxVkBuffer buffer);
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
extern u32 hash32(const GfxVkBuffer & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkTexture & it, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkDescriptorDesc & desc, u32 seed = MURMUR3_SEED);
extern u32 hash32(const GfxVkDescriptorSetDesc & desc, u32 seed =MURMUR3_SEED);
extern u32 hash32(const VkDescriptorSetLayoutCreateInfo & info, u32 seed =MURMUR3_SEED);

#endif // GFX_VULKAN_INTERNAL_H
