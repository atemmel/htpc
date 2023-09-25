#include "spotify_view.hpp"
#include "core/log.hpp"
#include "core/unit.hpp"
#include "spotify/spotify.hpp"
#include "ui/ui.hpp"
#include <cstdio>
#include <string>

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
