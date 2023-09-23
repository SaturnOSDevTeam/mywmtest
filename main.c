#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/Xcursor/Xcursor.h>

int main() {
    Display *display;
    Window root;
    XEvent event;

    /* 1. Initialize connection to the X server */
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Error connecting to the X server\n");
        return 1;
    }

    root = DefaultRootWindow(display);

    /* 2. Create initial window */
    Window window = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, 0), WhitePixel(display, 0));

    /* 3. Set up event handling */
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    /* 4. Set up mouse cursor */
    Cursor cursor = XCreateFontCursor(display, XC_left_ptr);
    XDefineCursor(display, window, cursor);

    /* Event loop */
    while (1) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                // Handle expose event
                break;
            case KeyPress:
                // Handle key press event
                break;
            default:
                break;
        }
    }

    /* 5. Clean up */
    XFreeCursor(display, cursor);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
