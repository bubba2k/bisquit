cmake_minimum_required(VERSION 3.10)

project(bisquit_interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bisquit_interpreter src/common.hpp)

target_sources(	bisquit_interpreter PRIVATE
				src/main.cpp
				src/parse.cpp
				src/runtime.cpp
				src/util.cpp
				src/keywords.cpp )

include_directories(${CMAKE_SOURCE_DIR})
