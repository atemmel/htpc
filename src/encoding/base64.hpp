#pragma once
#include <string>
#include <string_view>

namespace encoding {

auto toBase64(std::string_view view) -> std::string;

}
