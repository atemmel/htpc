cmake_minimum_required(VERSION 3.10)
project(htpc)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "./src/*.cpp")
add_executable(htpc ${SOURCES})
set_property(TARGET htpc PROPERTY CXX_STANDARD 20)
include_directories(src)

target_link_libraries(htpc SDL2 SDL2_ttf)
