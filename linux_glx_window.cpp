#include "window.h"

AppWindow* create_window(WindowCreateDesc desc)
{
    init_x11();
    PANIC_IF(!(desc.flags & WINDOW_OPENGL), "unsupported render backend");

    SArena scratch = tl_scratch_arena();
    AppWindow *wnd = ALLOC_T(mem_dynamic, AppWindow) {};

    int glx_major, glx_minor;
    PANIC_IF(!glXQueryVersion(x11.dsp, &glx_major, &glx_minor), "failed querying glx version");
    PANIC_IF(glx_major == 1 && glx_minor < 4, "unsupported version");
    LOG_INFO("GLX version: %d.%d", glx_major, glx_minor);

    i32 fb_attribs[] = {
        GLX_X_RENDERABLE    , 1,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , 1,
        0
    };

    int fb_count;
    GLXFBConfig *fbs = glXChooseFBConfig(x11.dsp, DefaultScreen(x11.dsp), fb_attribs, &fb_count);
    PANIC_IF(!fbs, "failed finding matching framebuffer configurations");

    i32 chosen_fb = -1, chosen_samples = -1;
    for (i32 i = 0; i < fb_count; i++) {
        i32 sample_buffers, samples;
        glXGetFBConfigAttrib(x11.dsp, fbs[i], GLX_SAMPLE_BUFFERS, &sample_buffers);
        glXGetFBConfigAttrib(x11.dsp, fbs[i], GLX_SAMPLES, &samples);

        if (sample_buffers && samples > chosen_samples) {
            chosen_fb = i;
            chosen_samples = samples;
        }
    }

    GLXFBConfig fbc = fbs[chosen_fb];
    XVisualInfo *vi = glXGetVisualFromFBConfig(x11.dsp, fbc);

    XSetWindowAttributes swa{};
    swa.colormap = XCreateColormap(x11.dsp, RootWindow(x11.dsp, vi->screen), vi->visual, AllocNone);

    wnd->handle = XCreateWindow(
        x11.dsp,
        RootWindow(x11.dsp, vi->screen),
        0, 0,
        desc.width, desc.height,
        0,
        vi->depth,
        InputOutput,
        vi->visual,
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

    LOAD_GL_PROC(glXCreateContextAttribsARB);

    i32 context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    GLXContext ctx = glXCreateContextAttribsARB(x11.dsp, fbc, 0, true, context_attribs);
    XSync(x11.dsp, false);

    glXMakeCurrent(x11.dsp, wnd->handle, ctx);

    load_opengl_procs();

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
    extern MouseCursor current_cursor;

    glXSwapBuffers(x11.dsp, wnd->handle);
    XDefineCursor(x11.dsp, wnd->handle, cursors[current_cursor]);
    current_cursor = MC_NORMAL;
}
