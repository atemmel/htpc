#include <catch2/catch.hpp>

#include "encoding/base64.hpp"

TEST_CASE("Base64 encode", "[]") {
	auto bytes = "5175";

	auto encoded = encoding::toBase64(bytes);

	REQUIRE(encoded == "NTE3NQ==");
}
