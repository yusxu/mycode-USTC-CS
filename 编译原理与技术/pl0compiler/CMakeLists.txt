cmake_minimum_required(VERSION 2.8)
project(pl0)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -std=c99")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c99")

add_subdirectory(lex)
add_subdirectory(common)
add_subdirectory(syntax)
add_subdirectory(error)
add_subdirectory(translation)

add_executable(pl0compiler test.c)
target_link_libraries(pl0compiler lex common tax syntax error expr translation)
