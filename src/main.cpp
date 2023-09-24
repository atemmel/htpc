#include "core/fs.hpp"
#include "http/http.hpp"
#include "core/log.hpp"
#include "core/math.hpp"
#include "spotify/spotify.hpp"
#include "steam/steam.hpp"
#include "spotify/auth.hpp"
#include "ui/ui.hpp"
#include "ui/list.hpp"
#include "ui/widget.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

/*
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
*/

/*
auto background_gradient_start = SDL_Color{
	.r = 255,
	.g = 44,
	.b = 112,
	.a = 255,
};

auto background_gradient_stop = SDL_Color{
	.r = 255,
	.g = 185,
	.b = 112,
	.a = 255,
};
*/

/*
auto background_gradient_start = SDL_Color{
	.r = 139,
	.g = 3,
	.b = 67,
	.a = 255,
};

auto background_gradient_stop = SDL_Color{
	.r = 105,
	.g = 0,
	.b = 5,
	.a = 255,
};
*/

auto background_gradient_start = SDL_Color{
	.r = 140,
	.g = 70,
	.b = 90,
	.a = 255,
};

auto background_gradient_stop = SDL_Color{
	.r = 75,
	.g = 30,
	.b = 75,
	.a = 255,
};

ui::Polygon background;

auto loopBody() -> void {
	ui::pollEvents();
	ui::clear();
	ui::draw(background);
	ui::drawWidgets();
	SDL_RenderPresent(ui::renderer);
};

auto steamGamesList() -> ui::Widget* {
	if(!steam::exists()) {
		return nullptr;
	}
	auto games = steam::games();
	auto options = std::vector<ui::List::Option>();
	for(auto& g : games) {
		auto appid = g.appid;
		options.push_back({
			.string = g.title,
			.fn = [appid](){
				const std::string base = "steam steam://run/";
				auto cmd = base + std::to_string(appid);
				system(cmd.c_str());
				return nullptr;
			},
		});
	}
	return ui::list(200, 100, options);
}

auto main() -> int {
	fs::createDirsIfNotAlreadyExists(fs::getProgramDataDir());

	if(!spotify::auth::hasPerformedFirstTimeAuth()) {
		spotify::auth::performFirstTimeAuth();
		return 0;
	} else {
		spotify::init();
	}
	return 0;
	ui::init();

	ui::active_widget = {
		ui::list(200, 100, {
			{"Start", steamGamesList, },
			{"Media", []() {
				println("media pressed");
				return nullptr;
			}},
			{"Settings",},
			{"A",},
			{"B",},
			{"C",},
			{"D",},
			{"E",},
			{"F",},
		}),
	};
	
	auto mode = ui::displayMode();
	
	auto w = mode.w;
	auto h = mode.h;
	println("resolution", w, "x", h);

	const auto start = background_gradient_start, stop = background_gradient_stop;

	auto halfway = SDL_Color{
		.r = (Uint8)std::lerp((float)start.r, (float)stop.r, 0.5f),
		.g = (Uint8)std::lerp((float)start.g, (float)stop.g, 0.5f),
		.b = (Uint8)std::lerp((float)start.b, (float)stop.b, 0.5f),
		.a = 255,
	};

	const float w_f = w, h_f = h;

	background = ui::Polygon{
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

	ui::run(loopBody);
	
	ui::quit();
}
