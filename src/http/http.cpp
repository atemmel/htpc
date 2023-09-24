#include "http.hpp"

#include <cstring>
#include <curl/curl.h>
#include <string>
#include <string_view>

#include "core/log.hpp"
#include "net/tcp_socket.hpp"

namespace http {

auto readRequest(TcpSocket client) -> Request {
	Request request;
	auto protocol = client.readUntil('\n');

	auto pathBegin = protocol.find(' ');
	auto pathEnd = protocol.find_last_of('?');
	auto query_end = protocol.find_last_of(' ');
	if(pathEnd == -1) {
		pathEnd = query_end;
	} else {
		size_t param_name_begin = pathEnd + 1;
		for(size_t i = param_name_begin; i < query_end; ) {
			std::string_view param_name;
			std::string_view param_value;
			for(; i < query_end; i++) {
				if(protocol[i] == '=') {
					param_name = std::string_view(
							protocol.begin() + param_name_begin,
							protocol.begin() + i);
					break;
				}
			}
			size_t param_value_begin = i + 1;
			for(; i < query_end; i++) {
				if(protocol[i] == '&' || protocol[i] == ' ') {
					break;
				}
			}

			param_value = std::string_view(
					protocol.begin() + param_value_begin,
					protocol.begin() + i);

			request.parameters.insert({
				std::string(param_name),
				std::string(param_value),
			});
		}
	}

	request.path = std::string(protocol.begin() + pathBegin + 1,
			protocol.begin() + pathEnd);

	while(true) {
		auto header = client.readUntil('\n');
		auto split = header.find(':');
		if(split == -1 || header.empty()) {
			break;
		}
	}
	return request;
}

auto writeHtmlResponse(TcpSocket client, std::string_view payload) -> void {
	client.write("HTTP/1.1 200 OK\r\n");
	client.write("Content-Type: text/html\r\n");
	client.write("Content-Length: ");
	client.write(std::to_string(payload.length()));
	client.write("\r\n");
	client.write("\r\n");
	client.write(payload);
}

auto appendParams(std::string& str, StaticPairs pairs) -> void {
	if(str.size() == 0 || pairs.size() == 0) {
		return;
	}

	str.push_back('?');
	for(const auto& p : pairs) {
		str.append(p.first);
		str.push_back('=');
		str.append(p.second);
		str.push_back('&');
	}
}

static auto writeCallback(char* data, size_t n, size_t l, void* userp) -> size_t {
	size_t real_size = n * l;
	auto response = reinterpret_cast<Response*>(userp);
	response->body.append(data, real_size);
	return real_size;
}

auto get(const std::string& url, StaticPairs headers) -> Response {
	Response response;

	curl_slist* header_list = nullptr;

	std::vector<std::string> joined_headers;
	for(auto header : headers) {
		joined_headers.emplace_back();
		auto joined = &joined_headers.back();
		joined->reserve(64);
		joined->append(header.first);
		joined->append(": ");
		joined->append(header.second);
		header_list = curl_slist_append(header_list, joined->c_str());
	}

	auto request = curl_easy_init();
	curl_easy_setopt(request, CURLOPT_URL, url.c_str());
	curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(request, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(request, CURLOPT_HTTPHEADER, header_list);

	curl_easy_perform(request);
	curl_slist_free_all(header_list);

	curl_easy_getinfo(request, CURLINFO_RESPONSE_CODE, &response.status);

	{
		curl_header *prev = nullptr;
		curl_header *h;
		 
		while((h = curl_easy_nextheader(request, CURLH_HEADER, 0, prev))) {
		   response.headers.insert({h->name, h->value});
		   prev = h;
		}
	}
	curl_easy_cleanup(request);
	return response;
}

auto post(const std::string& url, StaticPairs headers, std::string_view body) -> Response {
	Response response;

	curl_slist* header_list = nullptr;

	std::vector<std::string> joined_headers;
	for(auto header : headers) {
		joined_headers.emplace_back();
		auto joined = &joined_headers.back();
		joined->reserve(64);
		joined->append(header.first);
		joined->append(": ");
		joined->append(header.second);
		header_list = curl_slist_append(header_list, joined->c_str());
	}

	auto request = curl_easy_init();

	curl_easy_setopt(request, CURLOPT_URL, url.c_str());
	curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(request, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(request, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(request, CURLOPT_POSTFIELDS, body.data());
	curl_easy_setopt(request, CURLOPT_POSTFIELDSIZE, body.size());

	curl_easy_perform(request);
	curl_slist_free_all(header_list);

	curl_easy_getinfo(request, CURLINFO_RESPONSE_CODE, &response.status);

	{
		curl_header *prev = nullptr;
		curl_header *h;
		 
		while((h = curl_easy_nextheader(request, CURLH_HEADER, 0, prev))) {
		   response.headers.insert({h->name, h->value});
		   prev = h;
		}
	}
	curl_easy_cleanup(request);
	return response;
}

auto put(const std::string& url, StaticPairs headers, std::string_view body) -> Response{
	Response response;

	curl_slist* header_list = nullptr;

	std::vector<std::string> joined_headers;
	for(auto header : headers) {
		joined_headers.emplace_back();
		auto joined = &joined_headers.back();
		joined->reserve(64);
		joined->append(header.first);
		joined->append(": ");
		joined->append(header.second);
		header_list = curl_slist_append(header_list, joined->c_str());
	}

	auto request = curl_easy_init();

	curl_easy_setopt(request, CURLOPT_URL, url.c_str());
	curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(request, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(request, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(request, CURLOPT_POSTFIELDS, body.data());
	curl_easy_setopt(request, CURLOPT_POSTFIELDSIZE, body.size());
	curl_easy_setopt(request, CURLOPT_CUSTOMREQUEST, "PUT");

	curl_easy_perform(request);
	curl_slist_free_all(header_list);

	curl_easy_getinfo(request, CURLINFO_RESPONSE_CODE, &response.status);

	{
		curl_header *prev = nullptr;
		curl_header *h;
		 
		while((h = curl_easy_nextheader(request, CURLH_HEADER, 0, prev))) {
		   response.headers.insert({h->name, h->value});
		   prev = h;
		}
	}
	curl_easy_cleanup(request);
	return response;
}

auto operator<<(std::ostream& os, const Response& response) -> std::ostream& {
	return os << "\nstatus: " 
		<< response.status
		<< "\nheaders:"
		<< response.headers
		<< "body: "
		<< response.body;
}

auto operator<<(std::ostream& os, const Headers& headers) -> std::ostream& {
	for(const auto& h : headers) {
		os << "  \'" << h.first << "': " << h.second << "\n";
	}
	return os;
}

}
