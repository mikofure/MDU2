#include "x11_window.hpp"

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <cstring>
#include <cstdio>

// Setup basic X11 window properties for better desktop integration
void setup_x11_window_properties(void* display_ptr, unsigned long window) {
    Display* display = static_cast<Display*>(display_ptr);
    Window win = static_cast<Window>(window);
    
    if (!display || !win) {
        printf("Warning: Invalid X11 display or window\n");
        return;
    }
    
    // Set window class for better desktop integration
    set_x11_window_class(display, window, "MDU2");
    
    // Set additional window properties
    Atom wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    
    if (wm_protocols != None && wm_delete_window != None) {
        XSetWMProtocols(display, win, &wm_delete_window, 1);
    }
    
    // Set window type to normal application window
    Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    Atom net_wm_window_type_normal = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
    
    if (net_wm_window_type != None && net_wm_window_type_normal != None) {
        XChangeProperty(display, win, net_wm_window_type, XA_ATOM, 32,
                       PropModeReplace, (unsigned char*)&net_wm_window_type_normal, 1);
    }
    
    printf("X11 window properties configured\n");
}

// Set X11 window class for desktop environment integration
void set_x11_window_class(void* display_ptr, unsigned long window, const char* class_name) {
    Display* display = static_cast<Display*>(display_ptr);
    Window win = static_cast<Window>(window);
    
    if (!display || !win || !class_name) {
        return;
    }
    
    XClassHint class_hint;
    class_hint.res_name = const_cast<char*>(class_name);
    class_hint.res_class = const_cast<char*>(class_name);
    
    XSetClassHint(display, win, &class_hint);
}

// Set X11 window title
void set_x11_window_title(void* display_ptr, unsigned long window, const char* title) {
    Display* display = static_cast<Display*>(display_ptr);
    Window win = static_cast<Window>(window);
    
    if (!display || !win || !title) {
        return;
    }
    
    XStoreName(display, win, title);
    
    // Also set _NET_WM_NAME for UTF-8 support
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    
    if (net_wm_name != None && utf8_string != None) {
        XChangeProperty(display, win, net_wm_name, utf8_string, 8,
                       PropModeReplace, (unsigned char*)title, strlen(title));
    }
}

#else
// Stub implementations for non-Linux platforms
void setup_x11_window_properties(void* display, unsigned long window) {
    // No-op on non-Linux platforms
}

void set_x11_window_class(void* display, unsigned long window, const char* class_name) {
    // No-op on non-Linux platforms
}

void set_x11_window_title(void* display, unsigned long window, const char* title) {
    // No-op on non-Linux platforms
}
#endif