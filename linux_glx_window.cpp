#include "window.h"

AppWindow* create_window(WindowCreateDesc desc)
{
    SArena scratch = tl_scratch_arena();

    if (!(desc.flags & WINDOW_OPENGL)) PANIC("unsupported render backend");

    AppWindow *wnd = ALLOC_T(mem_dynamic, AppWindow) {
        .dsp = XOpenDisplay(0)
    };
    PANIC_IF(!wnd->dsp, "XOpenDisplay fail");

    int glx_major, glx_minor;
    PANIC_IF(!glXQueryVersion(wnd->dsp, &glx_major, &glx_minor), "failed querying glx version");
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
    GLXFBConfig *fbs = glXChooseFBConfig(wnd->dsp, DefaultScreen(wnd->dsp), fb_attribs, &fb_count);
    PANIC_IF(!fbs, "failed finding matching framebuffer configurations");

    i32 chosen_fb = -1, chosen_samples = -1;
    for (i32 i = 0; i < fb_count; i++) {
        i32 sample_buffers, samples;
        glXGetFBConfigAttrib(wnd->dsp, fbs[i], GLX_SAMPLE_BUFFERS, &sample_buffers);
        glXGetFBConfigAttrib(wnd->dsp, fbs[i], GLX_SAMPLES, &samples);

        if (sample_buffers && samples > chosen_samples) {
            chosen_fb = i;
            chosen_samples = samples;
        }
    }

    GLXFBConfig fbc = fbs[chosen_fb];
    XVisualInfo *vi = glXGetVisualFromFBConfig(wnd->dsp, fbc);

    XSetWindowAttributes swa{};
    swa.colormap = XCreateColormap(wnd->dsp, RootWindow(wnd->dsp, vi->screen), vi->visual, AllocNone);

    wnd->handle = XCreateWindow(
        wnd->dsp,
        DefaultRootWindow(wnd->dsp),
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

    XStoreName(wnd->dsp, wnd->handle, sz_string(desc.title, scratch));

    XSelectInput(
        wnd->dsp, wnd->handle,
        KeyPressMask | KeyReleaseMask |
        StructureNotifyMask |
        FocusChangeMask |
        PointerMotionMask | ButtonPressMask |
        LeaveWindowMask | EnterWindowMask |
        ButtonReleaseMask | EnterWindowMask);

    XMapWindow(wnd->dsp, wnd->handle);

    LOAD_GL_PROC(glXCreateContextAttribsARB);

    i32 context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    GLXContext ctx = glXCreateContextAttribsARB(wnd->dsp, fbc, 0, true, context_attribs);
    XSync(wnd->dsp, false);

    glXMakeCurrent(wnd->dsp, wnd->handle, ctx);

    load_opengl_procs();

    wnd->im = XOpenIM(wnd->dsp, NULL, NULL, NULL);
    wnd->ic = XCreateIC(wnd->im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, wnd->handle, NULL);

	if (!x11.xi_opcode) {
		// see http://who-t.blogspot.com/2009/05/xi2-recipes-part-1.html
		int event, error;
		PANIC_IF(!XQueryExtension(wnd->dsp, "XInputExtension", &x11.xi_opcode, &event, &error), "missing xi2 extension");

		int major = 2, minor = 0;
		PANIC_IF(XIQueryVersion(wnd->dsp, &major, &minor) == BadRequest, "failed querying XInput version");
		PANIC_IF(major < 2, "unsupported XInput extension version: %d.%d", major, minor);
		LOG_INFO("discovered xinput extension version %d.%d", major, minor);

		XIEventMask ximask{};
		unsigned char mask[XIMaskLen(XI_LASTEVENT)];
		memset(mask, 0, sizeof mask);

		ximask.deviceid = XIAllDevices;
		ximask.mask_len = sizeof mask;
		ximask.mask = mask;

		XISetMask(mask, XI_RawMotion);

		// Since raw events do not have target windows they are delivered exclusively to all root windows. Thus, a client that registers for raw events on a standard client window will receive a BadValue from XISelectEvents(). Like normal events however, if a client has a grab on the device, then the event is delivered only to the grabbing client.
		// see http://who-t.blogspot.com/2009/07/xi2-recipes-part-4.html
		XISelectEvents(wnd->dsp, DefaultRootWindow(wnd->dsp), &ximask, 1);
	}

	if (!x11.WM_PROTOCOLS) {
        X11_GET_ATOM(wnd->dsp, WM_PROTOCOLS);
        X11_GET_ATOM(wnd->dsp, WM_DELETE_WINDOW);

        Atom protocols[] = { x11.WM_DELETE_WINDOW };
        XSetWMProtocols(wnd->dsp, wnd->handle, protocols, ARRAY_COUNT(protocols));
    }

    if (!cursors[MC_NORMAL]) {
        cursors[MC_NORMAL] = XCreateFontCursor(wnd->dsp, XC_left_ptr);
        cursors[MC_SIZE_NW_SE] = XCreateFontCursor(wnd->dsp, XC_bottom_right_corner);

        XColor xcolor;
        char csr_bits[] = { 0x00 };

        Pixmap csr = XCreateBitmapFromData(wnd->dsp, wnd->handle, csr_bits, 1, 1);
        cursors[MC_HIDDEN] = XCreatePixmapCursor(wnd->dsp, csr, csr, &xcolor, &xcolor, 1, 1);
    }

    if (!clipboard.dsp) {
        clipboard.dsp = wnd->dsp;
        clipboard.wnd = XCreateSimpleWindow(
            clipboard.dsp,
            RootWindow(clipboard.dsp, DefaultScreen(clipboard.dsp)),
            -10, -10,
            1, 1,
            0, 0,
            0);
        PANIC_IF(!clipboard.wnd, "failed creading clipboard window");
        LOG_INFO("created clipboard wnd: %p", clipboard.wnd);

        clipboard.target_atom = XInternAtom(
            clipboard.dsp,
            "RAY_CLIPBOARD",
            False);
        clipboard.init = true;
    }

    return wnd;
}

void present_window(AppWindow *wnd)
{
    extern MouseCursor current_cursor;

    glXSwapBuffers(wnd->dsp, wnd->handle);
    XDefineCursor(wnd->dsp, wnd->handle, cursors[current_cursor]);
    current_cursor = MC_NORMAL;
}
