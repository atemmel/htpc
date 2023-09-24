#include "api.hpp"
#include "core/log.hpp"
#include "encoding/json.hpp"
#include "http/http.hpp"
#include "spotify/auth.hpp"
#include <cassert>
#include <string>
#include <string_view>

namespace spotify {

namespace api {

constexpr std::string_view baseUrl = "https://api.spotify.com/v1";

auto Device::fromJson(const json::Value *node) -> Device {
	assert(node != nullptr);

	Device device;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return device;
	}

	if(auto ptr = obj->lookupAsString("id"); ptr != nullptr) {
		device.id = ptr->value;
	}

	if(auto ptr = obj->lookupAsBool("is_active"); ptr != nullptr) {
		device.is_active = ptr->value;
	}

	if(auto ptr = obj->lookupAsBool("is_private_session"); ptr != nullptr) {
		device.is_private_session = ptr->value;
	}

	if(auto ptr = obj->lookupAsBool("is_restricted"); ptr != nullptr) {
		device.is_restricted = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("name"); ptr != nullptr) {
		device.name = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("type"); ptr != nullptr) {
		device.type = ptr->value;
	}

	if(auto ptr = obj->lookupAsInt("volume_percent"); ptr != nullptr) {
		device.volume_percent = ptr->value;
	}

	if(auto ptr = obj->lookupAsBool("supports_volume"); ptr != nullptr) {
		device.supports_volume = ptr->value;
	}

	return device;
}

auto AvailableDevices::fromJson(const json::Value *node) -> AvailableDevices {
	assert(node != nullptr);

	AvailableDevices availableDevices;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return availableDevices;
	}

	if(auto ptr = obj->lookupAsList("devices"); ptr != nullptr) {
		for(const auto& e : ptr->value) {
			availableDevices.devices.push_back(Device::fromJson(e.get()));
		}
	}

	return availableDevices;
}

auto getAvailableDevices(const auth::Context& ctx) -> AvailableDevices {
	static std::string url = std::string(baseUrl) + "/me/player/devices";
	auto response = http::get(url, {
		{"Authorization", ctx.bearerHeader()},
	});

	auto node = encoding::fromJson(response.body);
	auto availableDevices = AvailableDevices::fromJson(node.get());
	return availableDevices;
}

auto pause(const auth::Context& ctx, std::string_view id) -> void {
	static std::string url = std::string(baseUrl) + "/me/player/pause";
	http::put(url, {
		{"Authorization", ctx.bearerHeader()},
	}, id);
}

}

}
