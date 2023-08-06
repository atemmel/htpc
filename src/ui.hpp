#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ui {

struct Text {
	SDL_Texture* texture;
	SDL_Rect rect;

	auto destroy() -> void;
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;

auto init() -> void;

auto quit() -> void;

auto draw(const Text& text) -> void;

auto displayMode() -> SDL_DisplayMode;

auto text(TTF_Font* font, const char* text, SDL_Color color) -> Text;

}
