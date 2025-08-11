#include "darkmode.hpp"
#include <windows.h>
#include <dwmapi.h>

// Windows 10+ dark mode for titlebar
void enable_dark_mode(HWND hwnd) {
#if defined(_WIN32)
	// Try to enable dark mode for the window (Windows 10 1809+)
	const DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
	BOOL dark = TRUE;
	DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
#endif
}