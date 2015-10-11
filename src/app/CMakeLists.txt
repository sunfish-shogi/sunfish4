cmake_minimum_required(VERSION 2.8)

project(sunfish CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish
	main.cpp
)

target_link_libraries(sunfish core)
target_link_libraries(sunfish logger)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_link_libraries(sunfish wsock32)
endif()
