cmake_minimum_required(VERSION 2.8)

project(sunfish_test CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_test
	Main.cpp
	core/BitboardTest.cpp
  core/MoveTest.cpp
	core/PieceTest.cpp
	core/PositionTest.cpp
	core/SquareTest.cpp
)

target_link_libraries(sunfish_test core)
target_link_libraries(sunfish_test logger)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_link_libraries(sunfish_test wsock32)
endif()
