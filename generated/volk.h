#ifndef VOLK_GENERATED_H
#define VOLK_GENERATED_H

extern VkResult volkInitialize();
extern void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler);
extern void volkFinalize();
extern uint32_t volkGetInstanceVersion();
extern void volkLoadInstance(VkInstance instance);
extern void volkLoadInstanceOnly(VkInstance instance);
extern void volkLoadDevice(VkDevice device);
extern VkInstance volkGetLoadedInstance();
extern VkDevice volkGetLoadedDevice();
extern void volkLoadDeviceTable(struct VolkDeviceTable *table, VkDevice device);
extern VkResult volkInitialize();
extern void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler);
extern void volkFinalize();
extern uint32_t volkGetInstanceVersion();
extern void volkLoadInstance(VkInstance instance);
extern void volkLoadInstanceOnly(VkInstance instance);
extern VkInstance volkGetLoadedInstance();
extern void volkLoadDevice(VkDevice device);
extern VkDevice volkGetLoadedDevice();
extern void volkLoadDeviceTable(struct VolkDeviceTable *table, VkDevice device);

#endif // VOLK_GENERATED_H

#ifdef VOLK_GENERATED_IMPL
#define VOLK_INTERNAL
#endif

#if defined(VOLK_INTERNAL) && !defined(VOLK_INTERNAL_ONCE)
#define VOLK_INTERNAL_ONCE

static void volkGenLoadLoader(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadInstance(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadDevice(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadDeviceTable(struct VolkDeviceTable *table, void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static PFN_vkVoidFunction vkGetInstanceProcAddrStub(void *context, const char *name);
static PFN_vkVoidFunction vkGetDeviceProcAddrStub(void *context, const char *name);
static PFN_vkVoidFunction nullProcAddrStub(void *context, const char *name);
static void volkGenLoadLoader(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadInstance(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadDevice(void *context, PFN_vkVoidFunction (*)(void *, const char *)load);
static void volkGenLoadDeviceTable(struct VolkDeviceTable *table, void *context, PFN_vkVoidFunction (*)(void *, const char *)load);

#endif
