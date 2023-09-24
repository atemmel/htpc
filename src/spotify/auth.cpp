#include "auth.hpp"

#include "core/fs.hpp"
#include "data.hpp"
#include "core/log.hpp"
#include "encoding/base64.hpp"
#include "encoding/json.hpp"
#include "encoding/url.hpp"
#include "http/http.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>

//TODO
//auto state = generateRandomString(16);

namespace spotify {

namespace auth {

const std::string tokenUrl = "https://accounts.spotify.com/api/token";

auto authStepGetAuthLink(FirstTimeContext& ctx) -> void;
auto authStepGetCode(FirstTimeContext& ctx) -> void;
auto authStepGetTokenPair(FirstTimeContext& ctx) -> void;

auto Credentials::fromJson(json::Value* json) -> Credentials {
	assert(json != nullptr);

	Credentials auth;

	auto obj = json->tryObject();
	if(obj == nullptr) {
		return auth;
	}

	if(auto client_id = obj->lookup("client_id"); client_id != nullptr) {
		if(auto ptr = client_id->tryString(); ptr != nullptr) {
			auth.client_id = ptr->value;
		}
	}

	if(auto client_secret = obj->lookup("client_secret"); client_secret != nullptr) {
		if(auto ptr = client_secret->tryString(); ptr != nullptr) {
			auth.client_secret = ptr->value;
		}
	}

	return auth;
}

auto Token::fromJson(json::Value *json) -> Token {
	assert(json != nullptr);

	Token auth;

	auto obj = json->tryObject();
	if(obj == nullptr) {
		return auth;
	}

	if(auto access_token = obj->lookup("access_token"); access_token != nullptr) {
		if(auto ptr = access_token->tryString(); ptr != nullptr) {
			auth.access_token = ptr->value;
		}
	}

	if(auto refresh_token = obj->lookup("refresh_token"); refresh_token != nullptr) {
		if(auto ptr = refresh_token->tryString(); ptr != nullptr) {
			auth.refresh_token = ptr->value;
		}
	}

	if(auto token_type = obj->lookup("token_type"); token_type != nullptr) {
		if(auto ptr = token_type->tryString(); ptr != nullptr) {
			auth.token_type = ptr->value;
		}
	}

	if(auto scope = obj->lookup("scope"); scope != nullptr) {
		if(auto ptr = scope->tryString(); ptr != nullptr) {
			auth.scope = ptr->value;
		}
	}

	if(auto expires_in = obj->lookup("expires_in"); expires_in != nullptr) {
		if(auto ptr = expires_in->tryInt(); ptr != nullptr) {
			auth.expires_in = ptr->value;
		}
	}

	return auth;
}

auto Credentials::basicAuthHeader() const -> std::string {
	auto client_info_joined = this->client_id 
		+ ":" 
		+ this->client_secret;
	return  "Basic " + encoding::toBase64(client_info_joined);
}

auto Context::bearerHeader() const -> std::string {
	return "Bearer " + this->token.access_token;
}

auto credentialPath() -> std::string {
	static auto dir = fs::getProgramDataDir() + "/spotify_credentials.json";
	return dir;
}

auto tokenPath() -> std::string {
	static auto dir = fs::getProgramDataDir() + "/spotify_token.json";
	return dir;
}

auto hasPerformedFirstTimeAuth() -> bool {
	return std::filesystem::exists(tokenPath());
}

auto readToken() -> Token {
	auto token_node = encoding::fromJsonFile(tokenPath());
	return Token::fromJson(token_node.get());
}

auto readCredentials() -> Credentials {
	auto credential_node = encoding::fromJsonFile(credentialPath());
	return Credentials::fromJson(credential_node.get());
}

auto readCtx() -> Context {
	return Context{
		.credentials = readCredentials(),
		.token = readToken(),
	};
}

auto performFirstTimeAuth() -> void {
	auto port = 5445;
	auto credential_json = encoding::fromJsonFile(credentialPath());

	if(credential_json.get() == nullptr) {
		errprintln("Spotify credentials not set in:", credentialPath());
		return;
	}

	auto ctx = FirstTimeContext{
		.port = port,
		.our_redirect_uri = "http://localhost:" + std::to_string(port) + "/callback",
		.their_redirect_uri = "",
		.code = "",
		.auth_credentials = Credentials::fromJson(credential_json.get()),
		.auth_token = {},
	};

	authStepGetAuthLink(ctx);

	println("Opening", ctx.their_redirect_uri);
	//TODO: xdg-open (or equiv.)
	system(("xdg-open " + ctx.their_redirect_uri + " 2> /dev/null").c_str());

	authStepGetCode(ctx);
	authStepGetTokenPair(ctx);

	fs::writeAll(ctx.auth_token_raw, tokenPath());
	println("All ok!");
}

auto authStepGetAuthLink(FirstTimeContext& ctx) -> void {
	std::string url = "https://accounts.spotify.com/authorize";

	http::appendParams(url, {
		//{"state", state},
		{"client_id", ctx.auth_credentials.client_id},
		{"redirect_uri", encoding::toUrl(ctx.our_redirect_uri)},
		{"response_type", "code"},
		{"scope", data::scopes},
	});

	auto response = http::get(url);
	if(response.status != 303) {
		errprintln("authStepGetAuthLink failed...");
	}

	ctx.their_redirect_uri = response.headers["location"];
}

auto authStepGetCode(FirstTimeContext& ctx) -> void {
	auto server = TcpSocket::create();

	server.listen(ctx.port);

	for(auto callback_hit = false; !callback_hit;) {
		auto client = server.accept();
		auto request = http::readRequest(client);
		println(request.path);
		if(request.path == "/callback") {
			auto code_param = request.parameters.find("code");
			if(code_param == request.parameters.end()) {
				http::writeHtmlResponse(client, data::regular_payload);
			} else {
				ctx.code = code_param->second;
				http::writeHtmlResponse(client, data::callback_payload);
				callback_hit = true;
			}
		} else {
			http::writeHtmlResponse(client, data::regular_payload);
		}
		client.close();
	}
	server.close();

}

auto authStepGetTokenPair(FirstTimeContext& ctx) -> void {
	auto body = std::string("grant_type=authorization_code")
		+ "&code=" + ctx.code
		+ "&redirect_uri=" + encoding::toUrl(ctx.our_redirect_uri);

	auto auth_header = ctx.auth_credentials.basicAuthHeader();

	auto headers = http::StaticPairs{
		{"Authorization", auth_header},
		{"Content-Type", "application/x-www-form-urlencoded"},
	};

	auto response = http::post(tokenUrl, headers, body);
	println(response);
	auto node = encoding::fromJson(response.body);
	ctx.auth_token_raw = std::move(response.body);
	ctx.auth_token = Token::fromJson(node.get());
}

auto refresh(Context& context) -> void {
	auto body = std::string("grant_type=refresh_token")
		+ "&refresh_token=" + context.token.refresh_token;

	auto auth_header = context.credentials.basicAuthHeader();

	auto headers = http::StaticPairs{
		{"Authorization", auth_header},
		{"Content-Type", "application/x-www-form-urlencoded"},
	};

	auto response = http::post(tokenUrl, headers, body);
	auto node = encoding::fromJson(response.body);
	auto new_token = Token::fromJson(node.get());

	context.token.access_token = std::move(new_token.access_token);
	context.token.token_type = std::move(new_token.token_type);
	context.token.scope = std::move(new_token.scope);
	context.token.expires_in = new_token.expires_in;
}

}

}
