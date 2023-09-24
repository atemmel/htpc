#include "spotify.hpp"
#include "core/log.hpp"
#include "spotify/api.hpp"
#include "spotify/auth.hpp"
#include <cassert>

namespace spotify {

auth::Context ctx;

auto init() -> void {
	ctx = auth::readCtx();
	assert(ctx.token.access_token.size() > 0);
	assert(ctx.token.refresh_token.size() > 0);
	assert(ctx.credentials.client_id.size() > 0);
	assert(ctx.credentials.client_secret.size() > 0);

	auth::refresh(ctx);
	auto repsonse = api::getAvailableDevices(ctx);
	for(const auto& d : repsonse.devices) {
		if(d.name == "My htpc") {
			println("Found device!", d.id);
			api::pause(ctx, d.id);
			break;
		}
	}
}

}
