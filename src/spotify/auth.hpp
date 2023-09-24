#pragma once

#include "encoding/json_cpp.hpp"
#include <string>

namespace spotify {

namespace auth {

struct Credentials {
	std::string client_id;
	std::string client_secret;

	auto basicAuthHeader() const -> std::string;

	static auto fromJson(json::Value* json) -> Credentials;
};

struct Token {
	std::string access_token;
	std::string refresh_token;
	std::string token_type;
	std::string scope;
	int expires_in;

	static auto fromJson(json::Value* json) -> Token;
};

struct Context {
	Credentials credentials;
	Token token;

	auto bearerHeader() const -> std::string;
};

struct FirstTimeContext {
	int port;
	std::string our_redirect_uri;
	std::string their_redirect_uri;
	std::string code;
	Credentials auth_credentials;
	std::string auth_token_raw;
	Token auth_token;
};

auto credentialPath() -> std::string;

auto tokenPath() -> std::string;

auto hasPerformedFirstTimeAuth() -> bool;

auto readToken() -> Token;

auto readCredentials() -> Credentials;

auto readCtx() -> Context;

auto performFirstTimeAuth() -> void;

auto refresh(Context& context) -> void;

}

}
