#include "fstream"
#include "iostream"

using namespace std;

void read_vdf(string path) {
    ifstream stream;
    stream.open(path, std::ios::in);

    while (char c = stream.get()) 
        cout << c;
}

