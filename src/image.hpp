#pragma once

#include <SDL2/SDL.h>

namespace image {

auto gradient(SDL_Color start, SDL_Color stop, Uint32 w, Uint32 h) -> SDL_Surface*;

}
