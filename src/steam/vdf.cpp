#include <fstream>
#include <iostream>

void read_vdf(std::string path) {
	std::ifstream stream;
    stream.open(path, std::ios::in);

    while (char c = stream.get()) {
        std::cout << c;
	}
}

