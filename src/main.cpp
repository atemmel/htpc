#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <iostream>

auto main() -> int {
	SDL_Init(SDL_INIT_EVERYTHING); //TODO: check this
	TTF_Init();
	
	auto font = TTF_OpenFont("/usr/share/fonts/noto/NotoSans-Medium.ttf", 64);
	if(font == nullptr) {
		std::cerr << "Could not open font: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	SDL_DisplayMode mode;
	if(SDL_GetCurrentDisplayMode(0, &mode) != 0) {
		std::cerr << "Could not get current display mode: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}
	
	auto w = mode.w;
	auto h = mode.h;

	Uint32 window_flags = 0
			| SDL_WINDOW_SHOWN 
			| SDL_WINDOW_ALLOW_HIGHDPI 
			| SDL_WINDOW_ALWAYS_ON_TOP 
			| SDL_WINDOW_FULLSCREEN_DESKTOP;

	auto window = SDL_CreateWindow("htpc", 0, 0, w, h, window_flags);
	//TODO: check window
	
	Uint32 renderer_flags = 0
			| SDL_RENDERER_ACCELERATED 
			| SDL_RENDERER_PRESENTVSYNC;
	
	auto renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	//TODO: check renderer

	SDL_ShowCursor(SDL_DISABLE);

	auto surface = TTF_RenderText_Shaded(font, "My Button", {255, 255, 255, 255}, {0, 0, 0, 0});
	auto target = SDL_Rect{
		.x = 200,
		.y = 100,
		.w = surface->w,
		.h = surface->h,
	};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	
	auto running = true;
	while(running) {
		SDL_Event event = {0};
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_F12:
							running = false;
							break;
					}
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, &target);
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_ShowCursor(SDL_ENABLE);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}
