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

	/*
	auto background_surface = image::gradient(background_gradient_start, background_gradient_stop, w / 8, h / 8);

	auto background_texture = SDL_CreateTextureFromSurface(ui::renderer, background_surface);
	SDL_FreeSurface(background_surface);
	*/


	std::vector<std::string> menu = {
		"Start",
		"Media",
		"Settings",
	};

	std::vector<ui::Text> texts;
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

	size_t selection = 0;

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

	//auto circ = ui::circle(10, 10, SDL_FPoint{300, 300}, SDL_Color{255, 255, 255, 20});

	SDL_SetRenderDrawBlendMode(ui::renderer, SDL_BLENDMODE_BLEND);
	
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
							if(selection == 0) {
								break;
							}
							SDL_SetTextureColorMod(texts[selection].texture, 255, 255, 255);
							selection--;
							SDL_SetTextureColorMod(texts[selection].texture, 50, 50, 50);
							break;
						case SDLK_j:
							if(selection == texts.size() -1) {
								break;
							}
							SDL_SetTextureColorMod(texts[selection].texture, 255, 255, 255);
							selection++;
							SDL_SetTextureColorMod(texts[selection].texture, 50, 50, 50);
							break;
					}
					break;
			}
		}

		SDL_RenderClear(ui::renderer);
		ui::draw(background);

		auto sel_rect = SDL_Rect{
			.x = 100,
			.y = 0,
			.w = 200,
			.h = 100,
		};

		auto target_rect = &texts[selection];

		sel_rect.w = target_rect->rect.w;
		sel_rect.y = target_rect->rect.y;
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
