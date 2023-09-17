#include "steam.hpp"
#include "log.hpp"

#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace steam {

auto readAll(const char* file) -> std::string {
	std::ifstream stream;
	stream.open(file, std::ios::in | std::ios::binary | std::ios::ate);
	
	auto size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	if(size < 1) {
		return std::string();
	}

	std::vector<char> bytes(size);
	stream.read(bytes.data(), size);
	return std::string(bytes.data(), size);

}

auto readAcf(fs::path path) -> Game {
	auto src = readAll(path.c_str());
	auto findEnd = [&](std::string_view look_for) -> size_t {
		return src.find(look_for) 
			+ look_for.size();
	};

	auto seek = [&](char c, size_t from) -> size_t {
		for(auto i = from; i < src.size(); i++) {
			if(src[i] == c) {
				return i;
			}
		}
		assert(false);
	};

	auto getValue = [&](std::string_view value_name) -> std::string {
		auto idx = findEnd(value_name);
		auto value_begin = seek('"', idx) + 1;
		auto value_end = seek('"', value_begin);
		return src.substr(value_begin, value_end - value_begin);
	};

	uint64_t appid{};
	auto appid_str = getValue("\"appid\"");
	std::from_chars(appid_str.c_str(), appid_str.c_str() + appid_str.size(), appid);

	return {
		.title = getValue("\"name\""),
		.appid = appid,
	};
}

auto games() -> std::vector<Game> {
	fs::path home = getenv("HOME");
	fs::path steam_path = home / ".local/share/Steam/steamapps/";

	std::vector<Game> games;

	println("Looking for games in", steam_path.c_str());
	for(auto it : fs::directory_iterator(steam_path)) {
		if(it.is_directory()) {
			continue;
		}

		if(it.path().extension() == ".acf") {
			games.push_back(readAcf(it.path()));
		}
	}
	
	return games;
}

};

std::ostream& operator<<(std::ostream& os, const steam::Game& game) {
	return os << game.appid << " (" << game.title << ")";
}
