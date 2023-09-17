#pragma once
#include "steam/steam.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <utility>

#ifndef NDEBUG
#define println(...) _dprintImplBase(std::cout, __FILE__, __LINE__, __VA_ARGS__)
#else
#define println(...) {}
#endif

#define errprintln(...) _dprintImplBase(std::cerr, __FILE__, __LINE__, __VA_ARGS__)

#define fatal(x, ...) do { errprintln(__VA_ARGS__); std::exit(x); } while(false);

auto _dprintImpl(std::ostream& os) -> void;

template<typename First, typename... Rest>
auto _dprintImpl(std::ostream& os, First&& first, Rest&& ...rest) -> void {
	os << first << ' ';
	_dprintImpl(os, std::forward<Rest>(rest)...);
}

template<typename... Rest>
auto _dprintImplBase(std::ostream& os, const char* file, uint64_t line, Rest&& ...rest) -> void {
	os << file << " @ " << line << ": ";
	_dprintImpl(os, std::forward<Rest>(rest)...);
}
