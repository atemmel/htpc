#include "fs.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <system_error>
#include <vector>

namespace cfs = std::filesystem;

namespace fs {

auto getUserHome() -> std::string_view {
	return getenv("HOME");
}

auto getProgramDataDir() -> std::string {
	return std::string(getUserHome()) + "/.local/share/htpc";
}

auto createDirsIfNotAlreadyExists(std::string_view what) -> void {
	std::error_code err;
	cfs::create_directories(what, err);
}

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

auto readAll(std::string_view view) -> std::string {
	return readAll(view.data());
}

auto writeAll(std::string_view view, const char* file) -> void {
	std::ofstream stream(file, std::ios::out | std::ios::binary);
	stream.write(view.data(), view.size());
	stream.flush();
}

auto writeAll(std::string_view view, std::string_view file) -> void {
	writeAll(view, file.data());
}

}
