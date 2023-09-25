#pragma once

#include "api.hpp"
#include "auth.hpp"

namespace spotify {

auto init() -> void;

auto isInit() -> bool;

auto state() -> api::State;

auto play() -> void;

auto pause() -> void;

}
