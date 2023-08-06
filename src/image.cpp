#include "image.hpp"
#include <cmath>

namespace image {

auto gradient(SDL_Color start, SDL_Color stop, Uint32 w, Uint32 h) -> SDL_Surface* {
	auto surface = SDL_CreateRGBSurface(0, w, h,32,0,0,0,0);
	auto pixels = (Uint32*)surface->pixels;
	const float max = w + h;
	for(Uint32 y = 0; y < h; y++) {
		for(Uint32 x = 0; x < w; x++) {
			const float current = x + y;
			const float step = current / max;

			Uint8 r = std::round(std::lerp(start.r, stop.r, step));
			Uint8 g = std::round(std::lerp(start.g, stop.g, step));
			Uint8 b = std::round(std::lerp(start.b, stop.b, step));
			Uint8 a = std::round(std::lerp(start.a, stop.a, step));

			auto color_int = SDL_MapRGBA(surface->format, r, g, b, a);
			*pixels = color_int;
			pixels++;
		}
	}
	return surface;
}

}
