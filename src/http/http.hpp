#pragma once

#include "net/tcp_socket.hpp"

#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <string_view>

namespace http {

using StaticPairs = std::initializer_list<std::pair<std::string_view, std::string_view>>;

using Headers = std::map<std::string, std::string>;
using Parameters = std::map<std::string, std::string>;

enum struct Method {
	Get,
	Post,
	Put,
	Delete,
};

struct Request {
	std::string path;
	Parameters parameters;
	Headers headers;
	std::string body;
};

struct Response {
	int status;
	Headers headers;
	std::vector<std::byte> body;

	auto string() const -> std::string_view;
};

auto readRequest(TcpSocket client) -> Request;

auto writeHtmlResponse(TcpSocket client, std::string_view payload) -> void;

auto appendParams(std::string& str, StaticPairs pairs) -> void;

auto get(const std::string& url, StaticPairs headers = {}) -> Response;

auto post(const std::string& url, StaticPairs headers = {}, std::string_view body = {}) -> Response;

auto put(const std::string& url, StaticPairs headers = {}, std::string_view body = {}) -> Response;

auto operator<<(std::ostream& os, const Response& response) -> std::ostream&;
auto operator<<(std::ostream& os, const Headers& headers) -> std::ostream&;

}
