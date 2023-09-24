#pragma once
#include <cmath>
#include <limits>

constexpr static float Pi = 3.14159265359f;

template<typename T>
auto normalize(T current, T max) -> float {
	return (float)current / (float)max;
}

template<typename T>
auto within(T lower, T upper, T x) -> bool {
	return x < upper && x > lower;
}
