#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/keysym.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 50
#define ALT_KEY_MODIFIER (1 << 13)

void launchApplication(const char *command) {
    system(command);
}

void moveWindow(Display *display, Window window) {
    XEvent event;
    XGrabPointer(display, window, False, PointerMotionMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XAllowEvents(display, SyncPointer, CurrentTime);
    while (1) {
        XNextEvent(display, &event);
        if (event.type == ButtonRelease)
            break;
    }
    XUngrabPointer(display, CurrentTime);
}

void resizeWindow(Display *display, Window window) {
    XEvent event;
    XGrabPointer(display, window, False, PointerMotionMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XAllowEvents(display, SyncPointer, CurrentTime);
    while (1) {
        XNextEvent(display, &event);
        if (event.type == ButtonRelease)
            break;
    }
    XUngrabPointer(display, CurrentTime);
}

void closeWindow(Display *display, Window window) {
    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", True);
    event.xclient.format = 32;
    event.xclient.data.l[0] = XInternAtom(display, "WM_DELETE_WINDOW", True);
    event.xclient.data.l[1] = CurrentTime;
    XSendEvent(display, window, False, NoEventMask, &event);
}

void switchWindow(Display *display) {
    XAllowEvents(display, ReplayPointer, CurrentTime);
    XSync(display, False);
    XEvent event;
    event.xclient.type = KeyPress;
    event.xclient.send_event = True;
    event.xclient.display = display;
    event.xclient.window = DefaultRootWindow(display);
    event.xclient.message_type = XInternAtom(display, "_NET_CLIENT_LIST", False);
    event.xclient.format = 32;
    event.xclient.data.l[0] = SubstructureRedirectMask | SubstructureNotifyMask;
    event.xclient.data.l[1] = CurrentTime;
    XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
}

int main() {
    Display *display;
    Window root;
    XEvent event;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Error connecting to the X server\n");
        return 1;
    }

    root = DefaultRootWindow(display);

    Window window = XCreateSimpleWindow(display, root, 0, 0, 1920, 1080, 1, BlackPixel(display, 0), WhitePixel(display, 0));

    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask);
    XMapWindow(display, window);

    Cursor cursor = XCreateFontCursor(display, XC_left_ptr);
    XDefineCursor(display, window, cursor);

    while (1) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                // Handle expose event
                break;

            case KeyPress:
                if (event.xkey.state & ALT_KEY_MODIFIER) {
                    KeySym keySym = XKeycodeToKeysym(display, event.xkey.keycode, 0);
                    switch (keySym) {
                        case XK_F4:
                            closeWindow(display, window);
                            break;
                        
                        case XK_Tab:
                            switchWindow(display);
                            break;

                        default:
                            break;
                    }
                }
                break;

            case ButtonPress:
                if (event.xkey.state & ALT_KEY_MODIFIER) {
                    if (event.xbutton.button == Button1) {
                        moveWindow(display, window);
                    } else if (event.xbutton.button == Button3) {
                        resizeWindow(display, window);
                    }
                }
                break;

            default:
                break;
        }
    }

    XFreeCursor(display, cursor);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
