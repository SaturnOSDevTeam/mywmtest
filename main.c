#include <stdio.h>  // Include the <stdio.h> header for fprintf

#include <X11/Xlib.h>
#include <X11/Xutil.h>

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

    /* 4. Clean up */
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
