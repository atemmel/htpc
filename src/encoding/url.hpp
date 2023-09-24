#pragma once
#include <string>
#include <string_view>

namespace encoding {

auto toUrl(std::string_view src) ->std::string;

}
