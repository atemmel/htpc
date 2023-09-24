#include "url.hpp"

namespace encoding {

auto toUrl(std::string_view src) -> std::string {
	std::string dst;
	dst.reserve(src.size() * 2);
	auto allHex = "0123456789ABCDEF";

	for(auto c : src) {

		auto ok_char = ::isalnum(c) 
			|| c == '~' 
			|| c == '-' 
			|| c == '.' 
			|| c == '_';

		if(ok_char) {
			dst += c;
		} else {
			dst += '%';
			dst += allHex[c >> 4];
			dst += allHex[c & 15];
		}
	}
	return dst;
}

}
