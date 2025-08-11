#pragma once

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

// Linux X11 window management functions
void setup_x11_window_properties(void* display, unsigned long window);
void set_x11_window_class(void* display, unsigned long window, const char* class_name);
void set_x11_window_title(void* display, unsigned long window, const char* title);