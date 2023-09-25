#include "spotify.hpp"
#include "core/log.hpp"
#include "core/unit.hpp"
#include "spotify/api.hpp"
#include "spotify/auth.hpp"
#include <atomic>
#include <cassert>
#include <mutex>
#include <thread>

namespace spotify {

auth::Context ctx {};

bool init_success = false;

bool requesting = false;

api::State last_state {};
std::mutex last_state_mutex {};

auto refreshState() -> void;

auto activeDevice() -> api::Device&;

auto init() -> void {
	ctx = auth::readCtx();
	assert(ctx.token.access_token.size() > 0);
	assert(ctx.token.refresh_token.size() > 0);
	assert(ctx.credentials.client_id.size() > 0);
	assert(ctx.credentials.client_secret.size() > 0);

	auth::refresh(ctx);
	/*
	auto repsonse = api::getAvailableDevices(ctx);
	for(const auto& d : repsonse.devices) {
		if(d.name == "My htpc") {
			println("Found device!", d.id);
			api::play(ctx, d.id);
			break;
		}
	}

	auto state = api::state(ctx);
	println("Is playing?:", state.is_playing);
	println("ms:", state.progress_ms);

	auto ms = state.progress_ms;
	auto minutes = unit::msToFullMinutes(ms);
	auto seconds = unit::msToFullSeconds(ms);
	auto partial_seconds = unit::secondsNotPartOfMinute(seconds);

	println("Currently at:", minutes, "minutes and", partial_seconds, "seconds");
	*/

	std::thread pollState(refreshState);
	pollState.detach();

	init_success = true;
}

auto isInit() -> bool {
	return init_success;
}

auto state() -> api::State {
	return last_state;
}

auto play() -> void {
	if(requesting) {
		return;
	}
	requesting = true;
	std::thread th([]() {
		api::play(ctx, activeDevice().id);
		requesting = false;
	});
	th.detach();
}

auto pause() -> void {
	if(requesting) {
		return;
	}
	requesting = true;
	std::thread th([]() {
		api::pause(ctx, activeDevice().id);
		requesting = false;
	});
	th.detach();
}

auto refreshState() -> void {
	using namespace std::chrono_literals;

	while(true) {
		last_state_mutex.lock();
		last_state = api::state(ctx);
		last_state_mutex.unlock();
		std::this_thread::sleep_for(100ms);
	}
}

auto activeDevice() -> api::Device& {
	return last_state.device;
}

}
