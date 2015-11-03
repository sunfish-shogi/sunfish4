cmake_minimum_required(VERSION 2.8)

project(sunfish_dev CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_dev
	Main.cpp
  code_generator/ZobristCodeGenerator.cpp
)

target_link_libraries(sunfish_dev logger)
