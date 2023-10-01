#include "spotify_view.hpp"
#include "core/log.hpp"
#include "core/unit.hpp"
#include "spotify/spotify.hpp"
#include "ui/ui.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <string>

bool has_image = false;
ui::Image image;

SpotifyView::SpotifyView() {

}

SpotifyView::~SpotifyView() {

}

auto SpotifyView::draw() -> void {
	if(!spotify::isInit()) {
		auto text = ui::text(ui::font, "Spotify not loaded...", ui::fg);
		ui::draw(text, 200, 100);
		text.destroy();
		return;
	}

	state = spotify::state();

	if(state.is_playing && image.texture == nullptr) {
		auto maybe_bytes = spotify::getAlbumart(state.item.album);
		if (maybe_bytes) {
			auto& bytes = maybe_bytes.value;
			image = ui::image(bytes);
		}
	}

	std::string playing_text;
	playing_text = state.item.album.artists[0].name;
	for(size_t i = 1; i < state.item.album.artists.size(); i++) {
		playing_text += ", ";
		playing_text += state.item.album.artists[i].name;
	}

	playing_text += " - ";
	playing_text += state.item.name;

	int progress_minutes = unit::msToFullMinutes(state.progress_ms);
	int progress_as_seconds = unit::msToFullSeconds(state.progress_ms);
	int progress_seconds = unit::secondsNotPartOfMinute(progress_as_seconds);

	char buffer[6];

	sprintf(buffer, "%02d:%02d", progress_minutes, progress_seconds);

	auto playing_text_handle = ui::text(ui::font, playing_text.c_str(), ui::fg);
	auto time_text_handle = ui::text(ui::font, &buffer[0], ui::fg);

	ui::draw(image, 1100, 400);
	ui::draw(playing_text_handle, 200, 100);
	ui::draw(time_text_handle, 200, 200);

	playing_text_handle.destroy();
	time_text_handle.destroy();
}

auto SpotifyView::yes() -> void {
	if(!spotify::isInit()) {
		return;
	}

	if(!this->state.is_playing) {
		spotify::play();
	} else {
		spotify::pause();
	}
}
