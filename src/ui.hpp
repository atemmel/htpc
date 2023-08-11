#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>

namespace ui {

struct Text {
	SDL_Texture* texture;
	SDL_Rect rect;

	auto destroy() -> void;
};

struct Polygon {
	float offset_x = 0;
	float offset_y = 0;
	std::vector<SDL_Vertex> vertices;
	std::vector<int> indicies;

	auto move(float x, float y) -> void;

	auto setOffset(float x, float y) -> void;
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;

auto init() -> void;

auto quit() -> void;

auto pollAxis(Uint32 axis) -> float;

auto draw(const Text& text) -> void;

auto draw(const Polygon& polygon) -> void;

auto displayMode() -> SDL_DisplayMode;

auto circle(int n_segments, float r, SDL_FPoint center, SDL_Color color) -> ui::Polygon;

auto text(TTF_Font* font, const char* text, SDL_Color color) -> Text;

}
