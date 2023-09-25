#pragma once

#include <string>
namespace fs {

auto getUserHome() -> std::string_view;

auto getProgramDataDir() -> std::string;

auto getCacheDir() -> std::string;

auto createDirsIfNotAlreadyExists(std::string_view what) -> void;

auto readAll(const char* file) -> std::string;

auto readAll(std::string_view view) -> std::string;

auto writeAll(std::string_view view, const char* file) -> void;

auto writeAll(std::string_view view, std::string_view file) -> void;

}
