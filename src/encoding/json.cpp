#include "json.hpp"

#include "core/fs.hpp"
#include "encoding/json_cpp.hpp"
#include <cassert>
#include <string_view>

namespace encoding {

auto fromJson(std::string_view view) -> json::ValuePtr {
	json::Decoder decoder;
	return decoder.decode(view);
}

auto fromJsonFile(std::string_view path) -> json::ValuePtr {
	auto src = fs::readAll(path);
	return fromJson(src);
}

}
