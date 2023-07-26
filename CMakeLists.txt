cmake_minimum_required(VERSION 3.21)

project(Luntik)

file(GLOB_RECURSE SRCS ./src/*.cpp)

add_subdirectory(./libs/SFML/)
add_subdirectory(./libs/spdlog/)

link_libraries(sfml-system sfml-network sfml-graphics sfml-window)
link_libraries(spdlog)

add_executable(Luntik ${SRCS})

set_property(TARGET Luntik PROPERTY CXX_STANDARD 20)
