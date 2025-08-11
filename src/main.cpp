// main.cpp: SDL2 + Nuklear Cross-Platform Demo

#ifdef _WIN32
#include <windows.h>
#include "gui/win32/darkmode.hpp"
#elif __linux__
#include "gui/linux/x11_window.hpp"
#endif

#include <SDL.h>
#include <SDL_syswm.h>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "gui/renderer/nuklear_sdl_renderer.hpp"
#include "shared/fonts/uifont.hpp"
#include <stdio.h>

int main(int argc, char* argv[]) {
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}


	SDL_Window* window = SDL_CreateWindow(
		"MDU2 Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
	);
	if (!window) {
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Apply platform-specific window configuration
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(window, &wmInfo)) {
#ifdef _WIN32
		// Apply Win32 dark mode to the window
		HWND hwnd = wmInfo.info.win.window;
		enable_dark_mode(hwnd);
#elif __linux__
		// Configure X11 window properties
		if (wmInfo.subsystem == SDL_SYSWM_X11) {
			Display* display = wmInfo.info.x11.display;
			Window x11_window = wmInfo.info.x11.window;
			setup_x11_window_properties(display, x11_window);
			set_x11_window_title(display, x11_window, "MDU2 Demo - Linux X11");
		}
#endif
	}


	// Nuklear setup
	struct nk_context* ctx = nk_sdl_init(window, renderer);
	if (!ctx) {
		printf("Failed to initialize Nuklear context\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	
	// Initialize font atlas
	struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);
	
	// Load InterVariable font from embedded binary data
	printf("Loading InterVariable font from embedded binary data...\n");
	struct nk_font *font = nk_font_atlas_add_from_memory(atlas, (void*)resources_fonts_InterVariable_ttf, resources_fonts_InterVariable_ttf_len, 14.0f, NULL);
	if (!font) {
		printf("Warning: Failed to load embedded InterVariable font, using default font\n");
	} else {
		printf("Successfully loaded embedded InterVariable font\n");
	}
	
	nk_sdl_font_stash_end();
	
	// Set the font if loaded successfully
	if (font) {
		nk_style_set_font(ctx, &font->handle);
	}
	
	printf("Nuklear initialized successfully\n");

	bool running = true;
	SDL_Event evt;

	while (running) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) running = false;
			nk_sdl_handle_event(&evt);
		}

		nk_sdl_new_frame();

		// Get window dimensions
		int window_width, window_height;
		SDL_GetWindowSize(window, &window_width, &window_height);

		// Sidebar - Full height, 48px width, bg: #171717, border: #262626
		if (nk_begin(ctx, "Sidebar", nk_rect(0, 0, 48, window_height), NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BACKGROUND)) {
			// Set sidebar background color
			struct nk_style *s = &ctx->style;
			s->window.fixed_background = nk_style_item_color(nk_rgb(23, 23, 23)); // #171717
			s->window.border_color = nk_rgb(38, 38, 38); // #262626
			s->window.border = 1.0f;
			
			// Sidebar content can be added here
			nk_layout_row_static(ctx, 40, 40, 1);
			nk_button_label(ctx, "☰");
		}
		nk_end(ctx);

		// Main content area
		if (nk_begin(ctx, "Main", nk_rect(48, 0, window_width - 48, window_height), NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BACKGROUND)) {
			nk_layout_row_static(ctx, 30, 200, 1);
			nk_label(ctx, "Hello, Nuklear + SDL2!", NK_TEXT_CENTERED);
		}
		nk_end(ctx);

		// Render
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // dark background
		SDL_RenderClear(renderer);
		nk_sdl_render(NK_ANTI_ALIASING_ON);
		SDL_RenderPresent(renderer);
	}


	nk_sdl_shutdown();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}