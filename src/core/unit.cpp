#include "unit.hpp"

namespace unit {

auto msToFullMinutes(int ms) -> int {
	return (double(ms) / 60000.f);
}

auto msToFullSeconds(int ms) -> int {
	return double(ms) / 1000.f;
}

auto secondsNotPartOfMinute(int s) -> int {
	return s % 60;
}

}
