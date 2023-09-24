cmake_minimum_required(VERSION 3.10)

project(htpc)
project(TEST)

enable_testing()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(src)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "./src/*.cpp")
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "./tests/*.cpp")
list(REMOVE_ITEM SOURCES "src/main.cpp")

add_executable(htpc ${SOURCES} "src/main.cpp")
add_executable(TEST ${SOURCES} ${TEST_SOURCES})
set_property(TARGET htpc PROPERTY CXX_STANDARD 20)
set_property(TARGET TEST PROPERTY CXX_STANDARD 20)

target_link_libraries(htpc SDL2 SDL2_ttf curl)
target_link_libraries(TEST SDL2 SDL2_ttf curl)
