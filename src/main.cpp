#include "image.hpp"
#include "math.hpp"
#include "ui.hpp"

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

auto fg = SDL_Color{255, 255, 255, 255};

size_t selection = 0;

std::vector<ui::Text> texts;

auto select(size_t idx) -> void {
	SDL_SetTextureColorMod(texts[idx].texture, 50, 50, 50);
}

auto unselect(size_t idx) -> void {
	SDL_SetTextureColorMod(texts[idx].texture, 255, 255, 255);
}

auto up() -> void {
	if(selection == 0) {
		return;
	}
	unselect(selection);
	selection--;
	select(selection);
}

auto down() -> void {
	if(selection == texts.size() -1) {
		return;
	}
	unselect(selection);
	selection++;
	select(selection);
}

auto main() -> int {
	ui::init();
	
	auto font = TTF_OpenFont("/usr/share/fonts/noto/NotoSans-Medium.ttf", 64);
	if(font == nullptr) {
		std::cerr << "Could not open font: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	auto mode = ui::displayMode();
	
	auto w = mode.w;
	auto h = mode.h;

	std::vector<std::string> menu = {
		"Start",
		"Media",
		"Settings",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
	};

	texts.reserve(menu.size());
	{
		Uint32 y = 200;
		for(auto& str : menu) {
			texts.push_back(ui::text(font, str.c_str(), fg));
			texts.back().rect.x = 100;
			texts.back().rect.y = y;
			y += 100;
		}
	}

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

	auto marker_border = ui::circle(50, 50.f, {0.f, 0.f}, {255, 255, 255, 255});

	select(selection);

	Uint64 prev_axis_motion_timestamp = SDL_GetTicks64();
	float prev_axis_y = 0;
	bool traversing = false;
	Uint64 last_traversal = SDL_GetTicks64();
	
	auto running = true;
	while(running) {
		for(SDL_Event event; SDL_PollEvent(&event);) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_F12:
							running = false;
							break;
						case SDLK_k:
							up();
							break;
						case SDLK_j:
							down();
							break;
					}
					break;
			}
		}

		auto now = SDL_GetTicks64();
		auto axis_y = ui::pollAxis(1);
		constexpr float DEAD_ZONE = 0.3f;
		constexpr Uint64 TIME_DELTA = 1000;

		/*
		if(traversing) {
			constexpr Uint64 TRAVERSE_DELTA = 500;
			if(last_traversal + TRAVERSE_DELTA < now) {
				if(axis_y < -DEAD_ZONE) {
					up();
				} else if(axis_y > DEAD_ZONE) {
					down();
				}
			}
		}
		*/

		if(axis_y < -DEAD_ZONE) {
			if(within(-DEAD_ZONE, DEAD_ZONE, prev_axis_y)) {
				up();
			}
			/*
			if(prev_axis_motion_timestamp + TIME_DELTA < now) {
				traversing = true;
				prev_axis_motion_timestamp = now;
			}
			*/
		} else if(axis_y > DEAD_ZONE) {
			if(within(-DEAD_ZONE, DEAD_ZONE, prev_axis_y)) {
				down();
			}
			/*
			if(prev_axis_motion_timestamp + TIME_DELTA < now) {
				traversing = true;
				prev_axis_motion_timestamp = now;
			}
			*/
		} else {
			traversing = false;
		
		}
		prev_axis_y = axis_y;
		

		SDL_RenderClear(ui::renderer);
		ui::draw(background);
		
		auto target_rect = &texts[selection];

		auto sel_rect = SDL_Rect{
			.x = 100,
			.y = target_rect->rect.y,
			.w = target_rect->rect.w,
			.h = 100,
		};

		marker_border.setOffset(sel_rect.x, sel_rect.y + 50);
		ui::draw(marker_border);
		marker_border.setOffset(sel_rect.x + sel_rect.w, sel_rect.y + 50);
		ui::draw(marker_border);

		SDL_SetRenderDrawColor(ui::renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(ui::renderer, &sel_rect);

		for(auto& text : texts) {
			ui::draw(text);
		}
		
		SDL_RenderPresent(ui::renderer);
		SDL_Delay(16);
	}

	for(auto& text : texts) {
		text.destroy();
	}

	TTF_CloseFont(font);
	ui::quit();
}
