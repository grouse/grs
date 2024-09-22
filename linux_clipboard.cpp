#include <X11/Xatom.h>

struct {
	bool init = false;

    void *data = nullptr;
    i32 data_size = 0;
    Window wnd;

    Atom target_atom;
    bool pending = false;
} clipboard;

void handle_clipboard_events(XEvent event)
{
	if (!clipboard.init) return;

    switch (event.type) {
    case SelectionClear:
        ASSERT(event.xany.window == clipboard.wnd);
        LOG_INFO("selection ownership lost");
        FREE(mem_dynamic, clipboard.data);
        clipboard.data = nullptr;
        break;
    case SelectionRequest: {
            ASSERT(event.xany.window == clipboard.wnd);
            auto req = event.xselectionrequest;

            LOG_INFO(
                "SelectionRequest: requestor: %p, selection: %s, target: %s, property: %s",
                req.requestor,
                XGetAtomName(x11.dsp, req.selection),
                XGetAtomName(x11.dsp, req.target),
                XGetAtomName(x11.dsp, req.property));

            clipboard.pending = true;

            Atom XA_TARGETS = XInternAtom(x11.dsp, "TARGETS", False);
            Atom XA_UTF8_STRING = XInternAtom(x11.dsp, "UTF8_STRING", False);

            XEvent ev{};
            ev.xany.type = SelectionNotify;
            ev.xselection.selection = req.selection;
            ev.xselection.target = None;
            ev.xselection.property = None;
            ev.xselection.requestor = req.requestor;
            ev.xselection.time = req.time;

            if (req.target == XA_TARGETS) {
                Atom supported[] = {
                    XA_TARGETS,
                    XA_UTF8_STRING,
                };

                XChangeProperty(
                    x11.dsp,
                    req.requestor,
                    req.property,
                    XA_ATOM,
                    32,
                    PropModeReplace,
                    (unsigned char*)supported, ARRAY_COUNT(supported));

                ev.xselection.property = req.property;
                ev.xselection.target = XA_TARGETS;
            } else if (req.target == XA_UTF8_STRING) {
                XChangeProperty(
                    x11.dsp,
                    req.requestor,
                    req.property,
                    req.target,
                    8,
                    PropModeReplace,
                    (unsigned char*)clipboard.data, strlen((char*)clipboard.data));

                ev.xselection.target = req.target;
                ev.xselection.property = req.property;
                clipboard.pending = false;
            } else {
                LOG_ERROR("unhandled selection target");
            }

            XSendEvent(x11.dsp, req.requestor, False, NoEventMask, &ev);
            XSync(x11.dsp, False);
        } break;
    }
}

void set_clipboard_data(String str)
{
    char *clip = sz_string(str, mem_dynamic);
    char *e = atomic_exchange((char**)&clipboard.data, clip);

    XSetSelectionOwner(
        x11.dsp,
        XInternAtom(x11.dsp, "CLIPBOARD", False),
        clipboard.wnd,
        CurrentTime);

    if (e) FREE(mem_dynamic, e);
}

String read_clipboard_str(Allocator mem)
{
    // TODO(jesper): implement non-UTF8 clipboard text targets
    Atom XA_CLIPBOARD = XInternAtom(x11.dsp, "CLIPBOARD", false);
    Atom XA_UTF8 = XInternAtom(x11.dsp, "UTF8_STRING", false);

    Window owner = XGetSelectionOwner(x11.dsp, XA_CLIPBOARD);
    if (owner == None) return {};

    XConvertSelection(
        x11.dsp,
        XA_CLIPBOARD,
        XA_UTF8,
        XInternAtom(x11.dsp, "RAY_CLIPBOARD", False),
        clipboard.wnd,
        CurrentTime);

    XEvent event;
    // TODO(jesper): I don't think this is entirely correct. We need to
    // explicitly go into the clipboard.wnd's event queue, whereas this I think
    // will go into a bunch it shouldn't? It seems kinda dodge, and XWindowEvent
    // didn't seem to work for this? Maybe something with the event mask, which
    // there isn't one for selection events.
    while (XNextEvent(x11.dsp, &event) == 0) {
        switch (event.type) {
        case SelectionClear:
        case SelectionRequest:
            break;
        case SelectionNotify: {
            if (false) {
                LOG_INFO(
                         "SelectionNotify: requestor: %p, selection: %s, target: %s, property: %s",
                         event.xselection.requestor,
                         XGetAtomName(x11.dsp, event.xselection.selection),
                         XGetAtomName(x11.dsp, event.xselection.target),
                         XGetAtomName(x11.dsp, event.xselection.property));
            }

            if (event.xselection.property == None) break;
            Atom type, da;
            int di;
            unsigned long size, dul;
            unsigned char *prop_ret = nullptr;

            XGetWindowProperty(
                x11.dsp,
                clipboard.wnd,
                clipboard.target_atom,
                0, 0,
                False,
                AnyPropertyType,
                &type,
                &di,
                &dul,
                &size,
                &prop_ret);
            XFree(prop_ret);

            Atom incr = XInternAtom(x11.dsp, "INCR", False);
            PANIC_IF(incr == type, "data too large and INCR mechanism not implemented");

            XGetWindowProperty(
                x11.dsp,
                clipboard.wnd,
                clipboard.target_atom,
                0, size,
                False,
                AnyPropertyType,
                &da,
                &di,
                &dul,
                &dul,
                &prop_ret);

            String str = duplicate_string({(char*)prop_ret, (i32)strlen((char*)prop_ret) }, mem);
            if (false) LOG_INFO("clipboard content: %.*s", STRFMT(str));
            XFree(prop_ret);
            return str;
        } break;
        default:
            break;
        }
    }

    return {};
}
