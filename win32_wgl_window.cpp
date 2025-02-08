#include "window.h"

#include "win32_lite.h"
#include "win32_user32.h"
#include "win32_opengl.h"

AppWindow* create_window(WindowCreateDesc desc)
{
    PANIC_IF(!(desc.flags & WINDOW_OPENGL), "invalid render backend");

    SArena scratch = tl_scratch_arena();
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    DWORD style = WS_OVERLAPPEDWINDOW;

    AppWindow *wnd = ALLOC_T(mem_dynamic, AppWindow) {};
    defer { map_set(&windows, (u64)wnd->hwnd, wnd); };

    WNDCLASSA wc{
        .style = CS_OWNDC,
        .lpfnWndProc = &win32_window_proc,
        .hInstance = hInstance,
        .hCursor = LoadCursorA(NULL, IDC_ARROW),
        .lpszClassName = sz_string(desc.title, scratch),
    };

    RegisterClassA(&wc);

    HWND dummy_hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW,
        wc.lpszClassName,
        "dummy",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        desc.width, desc.height,
        NULL, NULL,
        hInstance,
        NULL);

    PANIC_IF(dummy_hwnd == NULL, "unable to create dummy window: (%d) %s", WIN32_ERR_STR);

    HDC dummy_hdc = GetDC(dummy_hwnd);
    PANIC_IF(dummy_hdc == NULL, "unable to get dc from dummy window");

    {
        PIXELFORMATDESCRIPTOR pfd{
            .nSize = sizeof pfd,
            .nVersion = 1,
            .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType = PFD_TYPE_RGBA,
            .cColorBits = 32,
            .cDepthBits = 24,
            .cStencilBits = 8,
            .iLayerType = PFD_MAIN_PLANE,
        };

        int format = ChoosePixelFormat(dummy_hdc, &pfd);
        if (!SetPixelFormat(dummy_hdc, format, &pfd)) PANIC("unable to set pixel format: (%d) %s", WIN32_ERR_STR);
    }

    HGLRC dummy_glrc = wglCreateContext(dummy_hdc);
    PANIC_IF(dummy_glrc == NULL, "unable to create gl context: (%d) %s", WIN32_ERR_STR);

    wglMakeCurrent(dummy_hdc, dummy_glrc);

    HMODULE gl_dll = LoadLibraryA("opengl32.dll");
    LOAD_GL_ARB_PROC(wglChoosePixelFormat, gl_dll);
    LOAD_GL_ARB_PROC(wglCreateContextAttribs, gl_dll);

    PANIC_IF(!wglChoosePixelFormat, "wglChoosePixelFormat proc not loaded, and no fallback available");
    PANIC_IF(!wglCreateContextAttribs, "wglCreateContextAttribs proc not loaded, and no fallback available");

    wnd->hwnd = CreateWindowExA(
        0,
        wc.lpszClassName,
        sz_string(desc.title, scratch),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        desc.width, desc.height,
        NULL, NULL,
        hInstance,
        NULL);
    PANIC_IF(wnd->hwnd == NULL, "unable to create window: (%d) %s", WIN32_ERR_STR);

    wnd->hdc = GetDC(wnd->hwnd);
    PANIC_IF(wnd->hdc == NULL, "unable to get dc from window");

    {
        i32 format_attribs[] = {
            WGL_DRAW_TO_WINDOW_ARB,           GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,           GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,            GL_TRUE,
            WGL_PIXEL_TYPE_ARB,               WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,               32,
            WGL_DEPTH_BITS_ARB,               24,
            WGL_STENCIL_BITS_ARB,             8,
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
            0
        };

        u32 formats_count;
        i32 format;
        wglChoosePixelFormat(wnd->hdc, format_attribs, NULL, 1, &format, &formats_count);

        PIXELFORMATDESCRIPTOR pfd = {};
        DescribePixelFormat(wnd->hdc, format, sizeof pfd, &pfd);
        SetPixelFormat(wnd->hdc, format, &pfd);
    }

    {
        i32 context_attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_DEBUG_BIT_ARB,
            0
        };

        wnd->glrc = wglCreateContextAttribs(wnd->hdc, 0, context_attribs);
    }

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummy_glrc);
    ReleaseDC(dummy_hwnd, dummy_hdc);
    DestroyWindow(dummy_hwnd);

    if (!wglMakeCurrent(wnd->hdc, wnd->glrc)) {
        PANIC("wglMakeCurrent failed");
    }

    load_opengl_procs(gl_dll);

    ShowWindow(wnd->hwnd, SW_SHOW);

    if (!cursors[MC_NORMAL]) {
        cursors[MC_NORMAL] = LoadCursorA(NULL, IDC_ARROW);
        cursors[MC_SIZE_NW_SE] = LoadCursorA(NULL, IDC_SIZENWSE);
    }

    return wnd;
}
