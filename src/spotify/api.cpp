#include "api.hpp"
#include "encoding/json.hpp"
#include "encoding/json_cpp.hpp"
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

auto Image::fromJson(const json::Value *node) -> Image {
	assert(node != nullptr);

	Image image;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return image;
	}

	if(auto ptr = obj->lookupAsInt("height"); ptr != nullptr) {
		image.height = ptr->value;
	}

	if(auto ptr = obj->lookupAsInt("width"); ptr != nullptr) {
		image.width = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("url"); ptr != nullptr) {
		image.url = ptr->value;
	}

	return image;
}

auto Artist::fromJson(const json::Value* node) -> Artist {
	assert(node != nullptr);

	Artist artist;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return artist;
	}

	if(auto ptr = obj->lookupAsString("name"); ptr != nullptr) {
		artist.name = ptr->value;
	}

	return artist;
}

auto Album::fromJson(const json::Value* node) -> Album {
	assert(node != nullptr);

	Album album;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return album;
	}

	if(auto ptr = obj->lookupAsString("album_type"); ptr != nullptr) {
		album.album_type = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("name"); ptr != nullptr) {
		album.name = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("release_date"); ptr != nullptr) {
		album.release_date = ptr->value;
	}

	if(auto ptr = obj->lookupAsInt("total_tracks"); ptr != nullptr) {
		album.total_tracks = ptr->value;
	}

	if(auto ptr = obj->lookupAsString("uri"); ptr != nullptr) {
		album.uri = ptr->value;
	}

	if(auto ptr = obj->lookupAsList("artists"); ptr != nullptr) {
		for(const auto& elem : ptr->value) {
			album.artists.push_back(Artist::fromJson(elem.get()));
		}
	}

	if(auto ptr = obj->lookupAsString("id"); ptr != nullptr) {
		album.id = ptr->value;
	}

	if(auto ptr = obj->lookupAsList("images"); ptr != nullptr) {
		for(const auto& elem : ptr->value) {
			album.images.push_back(Image::fromJson(elem.get()));
		}
	}

	return album;
}

auto Item::fromJson(const json::Value* node) -> Item {
	assert(node != nullptr);

	Item item;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return item;
	}

	if(auto ptr = obj->lookup("album"); ptr != nullptr) {
		item.album = Album::fromJson(ptr);
	}

	if(auto ptr = obj->lookupAsString("name"); ptr != nullptr) {
		item.name = ptr->value;
	}

	return item;
}

auto State::fromJson(const json::Value* node) -> State {
	assert(node != nullptr);

	State state;

	auto obj = node->tryObject();
	if(obj == nullptr) {
		return state;
	}

	if(auto ptr = obj->lookup("device"); ptr != nullptr) {
		state.device = Device::fromJson(ptr);
	}

	if(auto ptr = obj->lookupAsInt("timestamp"); ptr != nullptr) {
		state.timestamp = ptr->value;
	}

	if(auto ptr = obj->lookupAsInt("progress_ms"); ptr != nullptr) {
		state.progress_ms = ptr->value;
	}

	if(auto ptr = obj->lookupAsBool("is_playing"); ptr != nullptr) {
		state.is_playing = ptr->value;
	}

	if(auto ptr = obj->lookup("item"); ptr != nullptr) {
		state.item = Item::fromJson(ptr);
	}

	return state;
}

auto Play::toJson() const -> std::string {
	json::Encoder encoder;
	encoder.startEncoding();

	if(this->device_id) {
		encoder.insert("device_id", this->device_id.value);
	}

	if(this->context_uri) {
		encoder.insert("context_uri", this->context_uri.value);
	}

	if(this->uris) {
		encoder.insert("uris", this->uris.value);
	}

	/*
	 * TODO: hmmm....
	if(this->offset) {
		encoder.insert("offset", this->offset.value);
	}
	*/

	if(this->position_ms) {
		encoder.insert("position_ms", this->position_ms.value);
	}

	encoder.stopEncoding();
	return encoder.str();
}

auto getAvailableDevices(const auth::Context& ctx) -> AvailableDevices {
	const static std::string url = std::string(baseUrl) + "/me/player/devices";

	auto response = http::get(url, {
		{"Authorization", ctx.bearerHeader()},
	});

	auto node = encoding::fromJson(response.string());
	return AvailableDevices::fromJson(node.get());
}

auto getAlbumArt(const auth::Context& ctx, const Album& album) -> std::vector<std::byte> {
	size_t idx = 0;
	for(size_t i = 0; i < album.images.size(); i++) {
		if(album.images[i].width > album.images[idx].width) {
			idx = i;
		}
	}

	auto url = album.images[idx].url;
	auto response = http::get(url);
	return response.body;
}

auto state(const auth::Context& ctx) -> State {
	const static std::string url = std::string(baseUrl) + "/me/player";

	auto response = http::get(url, {
		{"Authorization", ctx.bearerHeader()},
	});

	auto node = encoding::fromJson(response.string());
	return State::fromJson(node.get());
}

auto pause(const auth::Context& ctx, std::string_view id) -> void {
	const static std::string url = std::string(baseUrl) + "/me/player/pause";

	http::put(url, {
		{"Authorization", ctx.bearerHeader()},
	}, id);
}

auto play(const auth::Context& ctx, std::string_view id) -> void {
	const static std::string url = std::string(baseUrl) + "/me/player/play";

	auto play = Play{
		.device_id = maybe(std::string(id)),
	};

	http::put(url, {
		{"Authorization", ctx.bearerHeader()},
	}, play.toJson());
}

}

}
