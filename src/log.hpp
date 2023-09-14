#pragma once
#include <cstdint>
#include <iostream>
#include <utility>

#ifndef NDEBUG
#define println(...) dprintImplBase(std::cout, __FILE__, __LINE__, __VA_ARGS__)
#else
#define println(...) {}
#endif

#define errprintln(...) dprintImplBase(std::cerr, __FILE__, __LINE__, __VA_ARGS__)

auto dprintImpl(std::ostream& os) -> void;

template<typename First, typename... Rest>
auto dprintImpl(std::ostream& os, First&& first, Rest&& ...rest) -> void {
	os << first << ' ';
	dprintImpl(os, std::forward<Rest>(rest)...);
}

template<typename... Rest>
auto dprintImplBase(std::ostream& os, const char* file, uint64_t line, Rest&& ...rest) -> void {
	os << file << " @ " << line << ": ";
	dprintImpl(os, std::forward<Rest>(rest)...);
}
