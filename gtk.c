#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <pixman.h>
#include <IL/il.h>

#define MAX_COMMAND_LENGTH 50

void launchApplication(const char *command) {
    system(command);
}

int main() {
    Display *display;
    Window root;
    XEvent event;

    ilInit();

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Error connecting to the X server\n");
        return 1;
    }

    root = DefaultRootWindow(display);

    Window window = XCreateSimpleWindow(display, root, 0, 0, 1280, 800, 1, BlackPixel(display, 0), WhitePixel(display, 0));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    Cursor cursor = XCreateFontCursor(display, XC_left_ptr);
    XDefineCursor(display, window, cursor);

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    ilLoadImage("~/mywmtest/images/minimalist-planets.jpg");
    ILenum error = ilGetError();
    if (error != IL_NO_ERROR) {
        fprintf(stderr, "Error loading background image: %s\n", iluErrorString(error));
        return 1;
    }

    int imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
    int imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    int imageBPP = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
    unsigned char *imageData = ilGetData();

    PixmanImage *pixmanImage = pixman_image_create_bits(
        PIXMAN_a8r8g8b8, imageWidth, imageHeight, (uint32_t *)imageData, imageBPP * imageWidth);

    if (!pixmanImage) {
        fprintf(stderr, "Error creating Pixman image\n");
        ilDeleteImages(1, &imageID);
        return 1;
    }

    while (1) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                XExposeEvent exposeEvent = event.xexpose;
                Drawable drawable = exposeEvent.window;

                cairo_t *cr = cairo_create(pixman_image_create_for_surface(pixmanImage));
                cairo_set_source_surface(cr, pixman_image_get_surface(pixmanImage), 0, 0);
                cairo_paint(cr);
                cairo_surface_destroy(cairo_get_target(cr));
                cairo_destroy(cr);

                XFlush(display);
                break;

            case KeyPress:
                if (event.xkey.state & ControlMask) {
                    if (event.xkey.keycode == XKeysymToKeycode(display, XK_N)) {
                        system("nemo &");
                    }
                    else if (event.xkey.keycode == XKeysymToKeycode(display, XK_T)) {
                        system("wezterm &");
                    }
                    else if (event.xkey.keycode == XKeysymToKeycode(display, XK_F)) {
                        system("firefox &");
                    }
                }
                break;

            default:
                break;
        }
    }

    ilDeleteImages(1, &imageID);
    pixman_image_unref(pixmanImage);

    XFreeCursor(display, cursor);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
