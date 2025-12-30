#ifndef GFX_VULKAN_GENERATED_H
#define GFX_VULKAN_GENERATED_H

extern Vector2 gfx_resolution();
extern void gfx_wait_frame();
extern void gfx_wait_for_frame();
extern GfxBuffer gfx_create_buffer(i32 size);
extern GfxBuffer gfx_create_buffer(void *data, i32 size);
extern GfxBuffer gfx_create_vertex_buffer(void *data, i32 size);
extern GfxBuffer gfx_create_index_buffer(void *data, i32 size);
extern GfxTexture gfx_load_texture(String path, bool sRGB = true);
extern GfxTexture gfx_load_texture(AssetHandle handle, bool sRGB = true);
extern void gfx_begin_pass(const GfxVkRenderPassDesc & desc);
extern void gfx_end_pass();
extern const char *sz_from_enum(GfxLoadOp op);
extern const char *sz_from_enum(GfxStoreOp op);

#endif // GFX_VULKAN_GENERATED_H

#if defined(GFX_VULKAN_INTERNAL) && !defined(GFX_VULKAN_INTERNAL_ONCE)
#define GFX_VULKAN_INTERNAL_ONCE

extern bool vk_image_format_supported(VkFormat format, VkImageCreateInfo image_info);
extern void vk_imm_begin();
extern void vk_imm_end();
extern void vk_destroy_swapchain();
extern void vk_create_swapchain(VkExtent2D extent);
extern void vk_recreate_swapchain();
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
extern void vk_update_uniform_buffer(GfxVkBuffer buffer, void *data, i32 size);
static VkAttachmentLoadOp vk_load_op(GfxLoadOp op);
static VkAttachmentStoreOp vk_store_op(GfxStoreOp op);
extern bool operator==(const VkExtensionProperties & lhs, const char *rhs);
extern bool operator==(const VkLayerProperties & lhs, const char *rhs);
extern bool operator==(const VkDescriptorSetLayoutCreateInfo & lhs, const VkDescriptorSetLayoutCreateInfo & rhs);
extern VkSampler vk_sampler(GfxSamplerDesc desc);
extern VkDescriptorSet vk_descriptor_set(GfxVkDescriptorSetDesc desc);
extern VkDescriptorSetLayout vk_descriptor_layout(FixedArray<VkDescriptorSetLayoutBinding, MAX_DESCRIPTOR_SET_BINDINGS> *bindings);
extern void vk_set_images(VkDescriptorSet set, u32 binding, u32 descriptor_count, GfxVkTexture *textures, VkSampler *samplers, VkImageLayout layout);
extern VkDescriptorSet vk_create_descriptor_set(VkDescriptorSetLayout layout);
extern VkBool32 vk_debug_proc(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *data, void *);
extern void vk_set_uniform(VkDescriptorSet set, u32 binding, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
extern GfxVkBuffer vk_material_parameters(GfxMaterialParameters params);
extern VkDescriptorPool vk_descriptor_pool(u32 set_count =100);
extern VkSurfaceKHR vk_create_surface(AppWindow *wnd, VkInstance instance);

#endif
