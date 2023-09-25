#include <catch2/catch.hpp>

#include "core/unit.hpp"

TEST_CASE("Time conversion", "[]") {
	auto ms = 160090;
	auto ms_as_minutes = unit::msToFullMinutes(ms);
	auto ms_as_seconds = unit::msToFullSeconds(ms);
	auto s = unit::secondsNotPartOfMinute(ms_as_seconds);

	REQUIRE(ms_as_minutes == 2);
	REQUIRE(ms_as_seconds == 160);
	REQUIRE(s == 40);
}
