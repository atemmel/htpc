#include <catch2/catch.hpp>

#include "core/log.hpp"
#include "encoding/json.hpp"
#include "encoding/json_cpp.hpp"

TEST_CASE("Json decode", "[]") {
	std::string_view view = 
"{\n"
  "\"devices\" : [ {\n"
  "} ]\n"
"}";

	auto node = encoding::fromJson(view);
	REQUIRE(node.get() != nullptr);

	auto obj = node->tryObject();
	REQUIRE(obj != nullptr);

	auto devices = obj->lookupAsList("devices");
	REQUIRE(devices != nullptr);

	REQUIRE(devices->value.size() == 1);
}
