#pragma once
#include "ui/widget.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
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

struct Image {
	SDL_Texture* texture;
	SDL_Rect rect;

	auto destroy() -> void;
};

const auto fg = SDL_Color{255, 255, 255, 255};

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern Widget* active_widget;

auto init() -> void;

auto run(void(*ptr)()) -> void;

auto quit() -> void;

auto pollEvents() -> void;

auto pollAxis(Uint32 axis) -> float;

auto clear() -> void;

auto drawWidgets() -> void;

auto draw(const Text& text) -> void;

auto draw(const Text& text, Uint32 x, Uint32 y) -> void;

auto draw(const Polygon& polygon) -> void;

auto draw(const Image& polygon, Uint32 x, Uint32 y) -> void;

auto displayMode() -> SDL_DisplayMode;

auto circle(int n_segments, float r, SDL_Color color) -> ui::Polygon;

auto text(TTF_Font* font, const char* text, SDL_Color color) -> Text;


auto registerWidget(std::unique_ptr<Widget>&& widget) -> Widget*;

}
