#pragma once
#include "core/maybe.hpp"
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

struct Offset {
	int position;
};

struct Play {
	Maybe<std::string> device_id;
	Maybe<std::string> context_uri;
	Maybe<std::vector<std::string>> uris;
	Maybe<Offset> offset;
	Maybe<int> position_ms;

	auto toJson() const -> std::string;
};

struct Image {
	int height;
	int width;
	std::string url;

	static auto fromJson(const json::Value* node) -> Image;
};

struct Artist {
	//external_urls
	//href
	//id
	std::string name;
	//type
	//uri

	static auto fromJson(const json::Value* node) -> Artist;
};

struct Album {
	std::string album_type;
	std::vector<Artist> artists;
	//avilable markets
	//external_urls
	//href
	std::string id;
	std::vector<Image> images;
	std::string name;
	std::string release_date;
	int total_tracks;
	std::string uri;

	static auto fromJson(const json::Value* node) -> Album;
};

struct Item {
	Album album;
	//artist;
	std::string name;

	static auto fromJson(const json::Value* node) -> Item;
};

struct State {
	Device device;
	std::string repeat_state;
	bool shuffle_state;
	// context
	int timestamp;
	int progress_ms;
	bool is_playing;
	Item item;
	// currently_playing_type
	// actions
	
	static auto fromJson(const json::Value* node) -> State;
};

auto getAvailableDevices(const auth::Context& ctx) -> AvailableDevices;

auto state(const auth::Context& ctx) -> State;

auto play(const auth::Context& ctx, std::string_view id) -> void;

auto pause(const auth::Context& ctx, std::string_view id) -> void;

}

}
