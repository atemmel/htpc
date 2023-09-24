#include "tcp_socket.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

auto TcpSocket::create() -> TcpSocket {
	TcpSocket tcpSocket;
	tcpSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpSocket.fd < 0) {
		//("Could not create socket");
	}

	return tcpSocket;
}

auto TcpSocket::close() const -> void {
	::close(fd);
}

auto TcpSocket::listen(uint16_t port) const -> void {
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		//("Failed to set SO_REUSEADDR option");
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
		//("Failed to set SO_REUSEPORT option");
	}

	int code = ::bind(fd, reinterpret_cast<const sockaddr*>(&hint), sizeof hint);
	if(code != 0) {
		//("Error binding port");
	}

	code = ::listen(fd, 256);

	if(code != 0) {
		//("Could not set socket into listening state");
	}
}

auto TcpSocket::accept() const -> TcpSocket {

	TcpSocket client;
	client.fd = ::accept(fd, nullptr, nullptr);
	if(client.fd == -1) {
		//("Failed to accept incoming connection");
	}

	return client;
}

auto TcpSocket::read(size_t howManyBytes) const -> std::string {
	std::string result;
	result.resize(howManyBytes, '\0');
	auto code = ::read(fd, result.data(), result.size());
	if(code < 0) {
		//("Reading from socket failed");
	}
	return result;
}

auto TcpSocket::readUntil(char thisByte) const -> std::string {
	std::string result;
	result.reserve(64);

	char byte = 0;
	while(true) {
		auto code = ::read(fd, &byte, 1);
		if(code < 0) {
			//("Reading from socket failed");
		}

		if(byte == thisByte) {
			return result;
		}

		result.push_back(byte);
	}
}

auto TcpSocket::readBytes(size_t howManyBytes) const -> std::vector<char> {
	std::vector<char> result;
	result.resize(howManyBytes);
	auto code = ::read(fd, result.data(), result.size());
	if(code < 0) {
		//("Reading from socket failed");
	}
	return result;
}

auto TcpSocket::write(std::string_view bytes) const -> size_t {
	auto code = ::write(fd, bytes.data(), bytes.size());
	if(code < 0) {
		//("Writing to socket failed");
	}
	return code;
}


auto TcpSocket::write(const std::vector<char>& bytes) const -> size_t {
	auto code = ::write(fd, bytes.data(), bytes.size());
	if(code < 0) {
		//("Writing to socket failed");
	}
	return code;
}

auto TcpSocket::operator<(TcpSocket rhs) const -> bool {
	return fd < rhs.fd;
}
