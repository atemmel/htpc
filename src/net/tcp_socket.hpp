#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct TcpSocket {
	[[nodiscard]] static auto create() -> TcpSocket;
	
	auto close() const -> void;
	auto listen(uint16_t port) const -> void;
	[[nodiscard]] auto accept() const -> TcpSocket;

	auto read(size_t howManyBytes) const -> std::string;
	auto readUntil(char thisByte) const -> std::string;
	auto readBytes(size_t howManyBytes) const -> std::vector<char>;
	auto write(std::string_view view) const -> size_t;
	auto write(const std::vector<char>& bytes) const -> size_t;

	auto operator<(TcpSocket rhs) const -> bool;
private:
	int fd;
};
