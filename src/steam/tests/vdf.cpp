#include "../vdf.hpp"
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("Print a vdf-file", "[]") {
    read_vdf("/home/arvid/.local/share/Steam/steamapps/libraryfolders.vdf");
}
