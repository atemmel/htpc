#pragma once

#include <SDL2/SDL.h>

namespace ui {

struct Widget {
	virtual ~Widget() = default;

	virtual auto draw() -> void = 0;
	virtual auto up() -> void {};
	virtual auto down() -> void {};

	Uint32 x = 0, y = 0;
};

}
