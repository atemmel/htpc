#include "image.hpp"
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

	auto background_surface = image::gradient(background_gradient_start, background_gradient_stop, w / 8, h / 8);

	auto background_texture = SDL_CreateTextureFromSurface(ui::renderer, background_surface);
	SDL_FreeSurface(background_surface);


	auto start_text = ui::text(font, "Start", fg);
	auto media_text = ui::text(font, "Media", fg);
	auto settings_text = ui::text(font, "Settings", fg);

	SDL_SetTextureColorMod(settings_text.texture, 0, 0, 0);

	start_text.rect.x = 100; start_text.rect.y = 200;
	media_text.rect.x = 100; media_text.rect.y = 300;
	settings_text.rect.x = 100; settings_text.rect.y = 400;

	size_t selection = 0;
	
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
							selection--;
							break;
						case SDLK_j:
							selection++;
							break;
					}
					break;
			}
		}

		SDL_RenderClear(ui::renderer);
		SDL_RenderCopy(ui::renderer, background_texture, nullptr, nullptr);

		auto sel_rect = SDL_Rect{
			.x = 100,
			.y = 0,
			.w = 200,
			.h = 100,
		};

		sel_rect.y = 200 + selection * 100;
		SDL_SetRenderDrawColor(ui::renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(ui::renderer, &sel_rect);

		ui::draw(start_text);
		ui::draw(media_text);
		ui::draw(settings_text);
		
		SDL_RenderPresent(ui::renderer);
		SDL_Delay(16);
	}

	SDL_DestroyTexture(background_texture);
	start_text.destroy();
	media_text.destroy();
	settings_text.destroy();
	TTF_CloseFont(font);
	ui::quit();
}
