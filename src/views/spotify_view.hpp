#pragma once

#include "spotify/api.hpp"
#include "ui/widget.hpp"

struct SpotifyView : public ui::Widget {

	SpotifyView();

	~SpotifyView();

	auto draw() -> void override;

	auto yes() -> void override;

	spotify::api::State state;
};
