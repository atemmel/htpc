#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
namespace steam {

struct Game {
	std::string title;
	uint64_t appid;
};

auto games() -> std::vector<Game>;

};

std::ostream& operator<<(std::ostream& os, const steam::Game& game);
