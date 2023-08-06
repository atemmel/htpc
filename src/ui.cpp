#include "ui.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <iostream>

SDL_Window* ui::window;
SDL_Renderer* ui::renderer;

namespace ui {

auto Text::destroy() -> void {
	SDL_DestroyTexture(this->texture);
}

auto init() -> void {
	SDL_Init(SDL_INIT_EVERYTHING); //TODO: check this
	TTF_Init();

	auto mode = displayMode();

	constexpr Uint32 window_flags = 0
			| SDL_WINDOW_ALLOW_HIGHDPI 
			| SDL_WINDOW_ALWAYS_ON_TOP 
			| SDL_WINDOW_FULLSCREEN_DESKTOP
			| SDL_WINDOW_SHOWN 
			| 0;

	window = SDL_CreateWindow("htpc", 0, 0, mode.w, mode.h, window_flags);
	//TODO: check window
	
	constexpr Uint32 renderer_flags = 0
			| SDL_RENDERER_ACCELERATED 
			| SDL_RENDERER_PRESENTVSYNC
			| 0;
	
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	//TODO: check renderer

	SDL_ShowCursor(SDL_DISABLE);
}

auto quit() -> void {
	SDL_ShowCursor(SDL_ENABLE);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

auto displayMode() -> SDL_DisplayMode {
	SDL_DisplayMode mode;
	if(SDL_GetCurrentDisplayMode(0, &mode) != 0) {
		std::cerr << "Could not get current display mode: " << SDL_GetError() << "\n";
		std::exit(EXIT_FAILURE);
	}
	return mode;
}

auto draw(const Text& text) -> void {
	SDL_RenderCopy(ui::renderer, text.texture, nullptr, &text.rect);
}

auto draw(const Polygon& polygon) -> void {
	SDL_RenderGeometry(
			ui::renderer, 
			nullptr, 
			polygon.vertices.data(), 
			polygon.vertices.size(), 
			polygon.indicies.data(), 
			polygon.indicies.size());

}

auto text(TTF_Font* font, const char* text, SDL_Color color) -> Text {
	auto surface = TTF_RenderUTF8_Blended(font, text, {255, 255, 255, 255});
	auto rect = SDL_Rect{
		.x = 0,
		.y = 0,
		.w = surface->w,
		.h = surface->h,
	};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return Text{
		.texture = texture,
		.rect = rect,
	};
}

}
