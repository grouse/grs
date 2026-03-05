#include "window.h"

#include "external/volk/volk.h"

VkSurfaceKHR vk_create_surface(AppWindow *wnd, VkInstance instance)
{
    VkSurfaceKHR surface;

    if (wnd->headless) {
        VkHeadlessSurfaceCreateInfoEXT create_info{
            VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT,
        };
        if (auto result = vkCreateHeadlessSurfaceEXT(instance, &create_info, NULL, &surface); result != VK_SUCCESS) {
            PANIC("unable to create headless surface: %d", result);
        }
        return surface;
    }

    VkXlibSurfaceCreateInfoKHR create_info{
        VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = x11.dsp,
        .window = wnd->handle,

    };

    if (auto result = vkCreateXlibSurfaceKHR(instance, &create_info, NULL, &surface); result != VK_SUCCESS) {
        PANIC("unable to create surface: %d", result);
    }
    return surface;
}

AppWindow* create_window(WindowCreateDesc desc)
{
    PANIC_IF(!(desc.flags & WINDOW_VULKAN), "unsupported render backend");

    AppWindow *wnd = ALLOC_T(mem_dynamic, AppWindow) {};
    wnd->client_resolution = { (f32)desc.width, (f32)desc.height };

    if (desc.flags & WINDOW_HEADLESS) {
        wnd->headless = true;
        return wnd;
    }

    init_x11();

    SArena scratch = tl_scratch_arena();

    Window parent = XDefaultRootWindow(x11.dsp);

    XSetWindowAttributes swa{};
    //swa.colormap = XCreateColormap(x11.dsp, parent, vi->visual, AllocNone);

    wnd->handle = XCreateWindow(
        x11.dsp, parent,
        0, 0,
        desc.width, desc.height,
        0,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWOverrideRedirect | CWBackPixmap | CWBorderPixel | CWColormap,
        &swa);

    PANIC_IF(!wnd->handle, "XCreateWindow failed");
    LOG_INFO("created wnd: %p", wnd);

    wnd->client_resolution = { (f32)desc.width, (f32)desc.height };
    XStoreName(x11.dsp, wnd->handle, sz_string(desc.title, scratch));

    XSelectInput(
        x11.dsp, wnd->handle,
        KeyPressMask | KeyReleaseMask |
        StructureNotifyMask |
        FocusChangeMask |
        PointerMotionMask | ButtonPressMask |
        LeaveWindowMask | EnterWindowMask |
        ButtonReleaseMask | EnterWindowMask);

    XMapWindow(x11.dsp, wnd->handle);

    wnd->im = XOpenIM(x11.dsp, NULL, NULL, NULL);
    wnd->ic = XCreateIC(wnd->im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, wnd->handle, NULL);

    Atom protocols[] = { x11.WM_DELETE_WINDOW };
    XSetWMProtocols(x11.dsp, wnd->handle, protocols, ARRAY_COUNT(protocols));

    init_cursors(wnd->handle);
    init_clipboard();
    return wnd;
}

void present_window(AppWindow *wnd)
{
    if (wnd->headless) return;

    extern MouseCursor current_cursor;

    XDefineCursor(x11.dsp, wnd->handle, cursors[current_cursor]);
    current_cursor = MC_NORMAL;
}
