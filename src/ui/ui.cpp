#include "embed/NotoSansMedium.hpp"
#include "core/log.hpp"
#include "core/math.hpp"
#include "ui.hpp"
#include "ui/list.hpp"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_surface.h>
#include <cassert>
#include <cstddef>
#include <string_view>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include "emscripten/html5.h"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <list>
#include <memory>

ui::Widget* ui::active_widget;
SDL_Window* ui::window;
SDL_Renderer* ui::renderer;
TTF_Font* ui::font;

std::list<std::unique_ptr<ui::Widget>> widgets;

struct PadState {
	Uint64 prev_axis_motion_timestamp = 0;
	float prev_axis_y = 0;
	bool repeat_axis = false;
	Uint64 last_repeat_axis = 0;
	SDL_GameController* internal = nullptr;
	bool a = false;
	bool prev_a = false;
};

PadState pad;
bool running = true;

namespace ui {
auto pollEventsImpl() -> bool;

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

auto Image::destroy() -> void {
	SDL_FreeSurface(this->surface);
	SDL_DestroyTexture(this->texture);
}

auto init() -> void {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS)) {
		fatal(1, "Could not init sdl:", SDL_GetError());
	}
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	TTF_Init();

	pad.prev_axis_motion_timestamp = SDL_GetTicks64();
	pad.last_repeat_axis = SDL_GetTicks64();

	auto rw = SDL_RWFromMem((char*)NotoSans_Medium_ttf, NotoSans_Medium_ttf_len);
	font = TTF_OpenFontRW(rw, 1, 64);

	/*
	font = TTF_OpenFont("/usr/share/fonts/noto/NotoSans-Medium.ttf", 64);
	if(font == nullptr) {
		std::cerr << "Could not open font: " << SDL_GetError() << "\n";
	}
	*/

	auto mode = displayMode();

	constexpr Uint32 window_flags = 0
			| SDL_WINDOW_ALLOW_HIGHDPI 
			| SDL_WINDOW_ALWAYS_ON_TOP 
#ifndef __EMSCRIPTEN__
			| SDL_WINDOW_FULLSCREEN_DESKTOP
#else
			| SDL_WINDOW_RESIZABLE
#endif
			| SDL_WINDOW_SHOWN 
			| 0;

	window = SDL_CreateWindow("htpc", 0, 0, mode.w, mode.h, window_flags);
	if(window == nullptr) {
		fatal(1, "Could not create window:", SDL_GetError());
	}
	
	constexpr Uint32 renderer_flags = 0
			| SDL_RENDERER_ACCELERATED 
			| SDL_RENDERER_PRESENTVSYNC
			| 0;
	
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	if(renderer == nullptr) {
		SDL_DestroyWindow(window);
		fatal(1, "Could not create renderer:", SDL_GetError());
	}

	//TODO: rethink for e.g 4K, scale retroactively
	//SDL_RenderSetLogicalSize(renderer, 1280, 800);
	//SDL_RenderSetLogicalSize(renderer, 3840, 2160);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if( SDL_NumJoysticks() < 1 ) {
		errprintln("Warning: No joysticks connected!");
	} else {
		pad.internal = SDL_GameControllerOpen(0);
		if(pad.internal == NULL) {
			errprintln("Warning: Unable to open game controller! SDL Error:", SDL_GetError());
		}
	}
}

auto run(void(*loop_body)()) -> void {
	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(loop_body, 0, 1);
#else
	while(running) {
		loop_body();
		//TODO: fix actual framerate-aware delay
		SDL_Delay(16);
	}
#endif
}

auto quit() -> void {
	widgets.clear();
	if(pad.internal != nullptr) {
		SDL_GameControllerClose(pad.internal);
	}
	SDL_ShowCursor(SDL_ENABLE);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

auto pollEvents() -> void {
	running = pollEventsImpl();
}

auto pollEventsImpl() -> bool {

#ifdef __EMSCRIPTEN__
	double w{}, h{};
	emscripten_get_element_css_size("#canvas", &w, &h);
	SDL_SetWindowSize(window, (int)w, (int)h );
#endif

	for(SDL_Event event; SDL_PollEvent(&event);) {
		switch (event.type) {
			case SDL_QUIT:
#ifdef __EMSCRIPTEN__
				emscripten_cancel_main_loop();
#endif
				return false;
				break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
					const auto w = event.window.data1;
					const auto h = event.window.data2;
					//SDL_SetWindowSize(window, w, h);
				}
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_F12:
						return false;
						break;
					case SDLK_k:
						active_widget->up();
						break;
					case SDLK_j:
						active_widget->down();
						break;
					case SDLK_KP_SPACE:
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						active_widget->yes();
						break;
				}
				break;
		}

		auto now = SDL_GetTicks64();
		auto axis_y = ui::pollAxis();
		constexpr float DEAD_ZONE = 0.1f;
		constexpr Uint64 TIME_DELTA = 500;

		pad.prev_a = pad.a;
		pad.a = SDL_GameControllerGetButton(pad.internal, SDL_CONTROLLER_BUTTON_A);

		if(!pad.a && pad.prev_a) {
			active_widget->yes();
		}

		if(pad.repeat_axis) {
			constexpr Uint64 TRAVERSE_DELTA = 100;
			if(pad.last_repeat_axis + TRAVERSE_DELTA < now) {
				pad.last_repeat_axis = now;
				if(axis_y < -DEAD_ZONE) {
					active_widget->up();
				} else if(axis_y > DEAD_ZONE) {
					active_widget->down();
				}
			}
		}

		if(axis_y < -DEAD_ZONE) {
			if(within(-DEAD_ZONE, DEAD_ZONE, pad.prev_axis_y)) {
				active_widget->up();
				pad.prev_axis_motion_timestamp = now;
			}
			if(pad.prev_axis_motion_timestamp + TIME_DELTA < now) {
				pad.repeat_axis = true;
				pad.prev_axis_motion_timestamp = now;
			}
		} else if(axis_y > DEAD_ZONE) {
			if(within(-DEAD_ZONE, DEAD_ZONE, pad.prev_axis_y)) {
				active_widget->down();
				pad.prev_axis_motion_timestamp = now;
			}
			if(pad.prev_axis_motion_timestamp + TIME_DELTA < now) {
				pad.repeat_axis = true;
				pad.prev_axis_motion_timestamp = now;
			}
		} else {
			pad.repeat_axis = false;
		
		}
		pad.prev_axis_y = axis_y;
		
	}
	return true;
}

auto pollAxis() -> float {
	auto p = SDL_GameControllerGetAxis(pad.internal, SDL_CONTROLLER_AXIS_LEFTY);
	return normalize(p, std::numeric_limits<Sint16>::max());
}

auto displayMode() -> SDL_DisplayMode {
	SDL_DisplayMode mode;
	if(SDL_GetCurrentDisplayMode(0, &mode) != 0) {
		errprintln("Could not get current display mode:", SDL_GetError());
		std::exit(EXIT_FAILURE);
	}
	return mode;
}

auto clear() -> void {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

auto drawWidgets() -> void {
	active_widget->draw();
}

auto draw(const Text& text) -> void {
	SDL_RenderCopy(ui::renderer, text.texture, nullptr, &text.rect);
}

auto draw(const Text& text, Uint32 x, Uint32 y) -> void{
	auto r = text.rect;
	r.x += x;
	r.y += y;
	SDL_RenderCopy(ui::renderer, text.texture, nullptr, &r);
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

auto draw(const Image& image, int x, int y) -> void {
	auto r = image.rect;
	r.x += x;
	r.y += y;
	SDL_RenderCopy(ui::renderer, image.texture, nullptr, &r);
}

auto circle(int n_segments, float r, SDL_Color color) -> ui::Polygon {
	auto center = SDL_FPoint{ 0.f, 0.f};
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

auto image(std::string_view path) -> Image {
	auto surface = IMG_Load(path.data());
	assert(surface != nullptr);
	auto rect = SDL_Rect{
		.x = 0,
		.y = 0,
		.w = surface->w,
		.h = surface->h,
	};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	return Image {
		.surface = surface,
		.texture = texture,
		.rect = rect,
	};
}

auto image(const std::vector<std::byte>& bytes) -> Image {
	auto rw = SDL_RWFromConstMem(bytes.data(), bytes.size());
	auto surface = IMG_Load_RW(rw, 1);
	assert(surface != nullptr);
	auto rect = SDL_Rect{
		.x = 0,
		.y = 0,
		.w = surface->w,
		.h = surface->h,
	};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	return Image {
		.surface = surface,
		.texture = texture,
		.rect = rect,
	};
}

auto registerWidget(std::unique_ptr<Widget>&& widget) -> Widget* {
	widgets.push_back(std::move(widget));
	return widgets.back().get();
}

}
