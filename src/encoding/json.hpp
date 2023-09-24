#pragma once

#include "encoding/json_cpp.hpp"
namespace encoding {

auto fromJson(std::string_view view) -> json::ValuePtr;

auto fromJsonFile(std::string_view path) -> json::ValuePtr;

auto getBool(const json::ObjectValue* node, std::string_view key) -> const json::BoolValue*;

auto getString(const json::ObjectValue* node, std::string_view key) -> const json::StringValue*;

auto getInt(const json::ObjectValue* node, std::string_view key) -> const json::IntValue*;

auto getFloat(const json::ObjectValue* node, std::string_view key) -> const json::FloatValue*;

auto getList(const json::ObjectValue* node, std::string_view key) -> const json::ListValue*;

auto getObject(const json::ObjectValue* node, std::string_view key) -> const json::ObjectValue*;

}
