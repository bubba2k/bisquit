cmake_minimum_required(VERSION 3.10)

project(biscuit_interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(biscuit_interpreter src/common.hpp)

target_sources(	biscuit_interpreter PRIVATE
				src/main.cpp
				src/parse.cpp
				src/keywords.cpp )

include_directories(${CMAKE_SOURCE_DIR})
