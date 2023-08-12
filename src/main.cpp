#include "image.hpp"
#include "math.hpp"
#include "ui.hpp"
#include "ui/list.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <string>
#include <vector>

auto background_gradient_start = SDL_Color{
	.r = 50,
	.g = 0,
	.b = 0,
	.a = 255,
};

auto background_gradient_stop = SDL_Color{
	.r = 105,
	.g = 0,
	.b = 105,
	.a = 255,
};

auto main() -> int {
	ui::init();

	ui::active_widget = {
		ui::list(200, 100, {
			"Start",
			"Media",
			"Settings",
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
		}),
	};
	
	auto mode = ui::displayMode();
	
	auto w = mode.w;
	auto h = mode.h;

	const auto start = background_gradient_start, stop = background_gradient_stop;

	auto halfway = SDL_Color{
		.r = (Uint8)std::lerp((float)start.r, (float)stop.r, 0.5f),
		.g = (Uint8)std::lerp((float)start.g, (float)stop.g, 0.5f),
		.b = (Uint8)std::lerp((float)start.b, (float)stop.b, 0.5f),
		.a = 255,
	};

	const float w_f = w, h_f = h;

	auto background = ui::Polygon{
		.vertices = {
			{ SDL_FPoint{0.f, 0.f}, start, SDL_FPoint{0}},
			{ SDL_FPoint{0.f, h_f}, halfway, SDL_FPoint{0}},
			{ SDL_FPoint{w_f, h_f}, stop, SDL_FPoint{0}},
			{ SDL_FPoint{w_f, 0.f}, halfway, SDL_FPoint{0}},
		},
		.indicies = {
			0, 1, 2, 
			0, 2, 3,
		},

	};
	
	auto running = true;
	while(running) {
		running = ui::pollEvents();
		SDL_RenderClear(ui::renderer);
		ui::draw(background);
		ui::drawWidgets();
		SDL_RenderPresent(ui::renderer);
		SDL_Delay(16);
	}

	ui::quit();
}
