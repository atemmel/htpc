#include <catch2/catch.hpp>

#include "steam/vdf.hpp"

TEST_CASE("Print a vdf-file", "[]") {
    read_vdf("/home/arvid/.local/share/Steam/steamapps/libraryfolders.vdf");
}
