#include "math.hpp"
#include "ui.hpp"

#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <iostream>

SDL_Window* ui::window;
SDL_Renderer* ui::renderer;

SDL_Joystick* pad = nullptr;

namespace ui {

auto Text::destroy() -> void {
	SDL_DestroyTexture(this->texture);
}

auto Polygon::move(float x, float y) -> void {
	offset_x += x;
	offset_y += y;

	for(auto& v : vertices) {
		v.position.x += x;
		v.position.y += y;
	}
}

auto Polygon::setOffset(float x, float y) -> void {
	for(auto& v : vertices) {
		v.position.x += x - offset_x;
		v.position.y += y - offset_y;
	}
	
	offset_x = x;
	offset_y = y;
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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if( SDL_NumJoysticks() < 1 ) {
		printf( "Warning: No joysticks connected!\n" );
	} else {
		pad = SDL_JoystickOpen(0);
		if(pad == NULL) {
			printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
		}
	}
}

auto pollAxis(Uint32 axis) -> float {
	auto p = SDL_JoystickGetAxis(pad, axis);
	return normalize(p, std::numeric_limits<Sint16>::max());
}

auto quit() -> void {
	if(pad != nullptr) {
		SDL_JoystickClose(pad);

	}
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

auto draw(const Polygon& polygon, int x, int y) -> void {
	SDL_RenderGeometry(
			ui::renderer, 
			nullptr, 
			polygon.vertices.data(), 
			polygon.vertices.size(), 
			polygon.indicies.data(), 
			polygon.indicies.size());

}

auto circle(int n_segments, float r, SDL_FPoint center, SDL_Color color) -> ui::Polygon {
	ui::Polygon shape;
    shape.vertices.resize(n_segments + 1);
    float segRotationAngle = (360.f / n_segments) * (Pi / 180.f);
    shape.vertices[0].position = center;
    shape.vertices[0].color = color;

    float startX = 0.f - r;
    float startY = 0.f;

    for(int i = 1; i < n_segments + 1; i++) {
        float finalSegRotationAngle = (i * segRotationAngle);
        shape.vertices[i].position.x = cos(finalSegRotationAngle) * startX - sin(finalSegRotationAngle) * startY;
        shape.vertices[i].position.y = cos(finalSegRotationAngle) * startY + sin(finalSegRotationAngle) * startX;

        shape.vertices[i].position.x += center.x;
        shape.vertices[i].position.y += center.y;
        shape.vertices[i].color = color;

        shape.indicies.push_back(0);
        shape.indicies.push_back(i);

        int index = (i + 1) % n_segments;
        if (index == 0)
        {
            index = n_segments;
        }
        shape.indicies.push_back(index);
    }

	return shape;
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
