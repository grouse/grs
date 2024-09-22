#include "window.h"
#include "win32_lite.h"
#include "win32_user32.h"

#include "external/volk/volk.h"

AppWindow* create_window(WindowCreateDesc desc)
{
    PANIC_IF(!(desc.flags & WINDOW_VULKAN), "invalid render backend");

    SArena scratch = tl_scratch_arena();
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD ex_style = WS_EX_TOPMOST;

    auto *wnd = ALLOC_T(mem_dynamic, AppWindow) {};
    defer { map_set(&windows, (u64)wnd->hwnd, wnd); };

    WNDCLASSA wc{
        .style = CS_OWNDC,
        .lpfnWndProc = &win32_window_proc,
        .hInstance = hInstance,
        .hCursor = LoadCursorA(NULL, IDC_ARROW),
        .lpszClassName = sz_string(desc.title, scratch),
    };

    RegisterClassA(&wc);

    wnd->hwnd = CreateWindowExA(
        ex_style,
        wc.lpszClassName,
        sz_string(desc.title, scratch),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        desc.width, desc.height,
        NULL, NULL,
        hInstance,
        NULL);
    PANIC_IF(wnd->hwnd == NULL, "unable to create window: (%d) %s", WIN32_ERR_STR);

    ShowWindow(wnd->hwnd, SW_SHOW);

    if (!cursors[MC_NORMAL]) {
        cursors[MC_NORMAL] = LoadCursorA(NULL, IDC_ARROW);
        cursors[MC_SIZE_NW_SE] = LoadCursorA(NULL, IDC_SIZENWSE);
    }

    return wnd;
}

VkSurfaceKHR vk_create_surface(AppWindow *wnd, VkInstance instance)
{
    VkSurfaceKHR surface;

    VkWin32SurfaceCreateInfoKHR create_info{
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = GetModuleHandleA(NULL),
        .hwnd = wnd->hwnd,
    };

    if (auto result = vkCreateWin32SurfaceKHR(instance, &create_info, NULL, &surface); result != VK_SUCCESS) {
        PANIC("unable to create surface: %d", result);
    }
    return surface;
}
