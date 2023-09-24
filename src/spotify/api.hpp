#pragma once
#include "spotify/auth.hpp"
#include <vector>

namespace spotify {

namespace api {

struct Device {
	std::string id;
	bool is_active;
	bool is_private_session;
	bool is_restricted;
	std::string name;
	std::string type;
	int volume_percent;
	bool supports_volume;

	static auto fromJson(const json::Value* node) -> Device;
};

struct AvailableDevices {
	std::vector<Device> devices;

	static auto fromJson(const json::Value* node) -> AvailableDevices;
};

auto getAvailableDevices(const auth::Context& ctx) -> AvailableDevices;

auto pause(const auth::Context& ctx, std::string_view id) -> void;

}

}
