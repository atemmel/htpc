#pragma once

#include "api.hpp"
#include "auth.hpp"
#include "core/maybe.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace spotify {

struct Cache {
	std::unordered_map<std::string, std::vector<std::byte>> map;

	auto toJson() -> std::string  { 
		//TODO: this
		return {};
	}
	static auto fromJson(const json::Value* node) -> Cache {
		//TODO: this
		return {};
	}
};

auto init() -> void;

auto isInit() -> bool;

auto state() -> api::State;

auto play() -> void;

auto pause() -> void;

auto getAlbumart(const api::Album& album) -> Maybe<std::vector<std::byte>>;

}
