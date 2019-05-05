cmake_minimum_required(VERSION 2.8)

project(sunfish_tools CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../book "${CMAKE_CURRENT_BINARY_DIR}/book")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")

add_executable(sunfish_tools
    Main.cpp
    csa2kifu/Csa2Kifu.cpp
    csa2kifu/Csa2Kifu.hpp
    sfen2csa/Sfen2Csa.cpp
    sfen2csa/Sfen2Csa.hpp
)

target_link_libraries(sunfish_tools book)
target_link_libraries(sunfish_tools core)
target_link_libraries(sunfish_tools logger)
target_link_libraries(sunfish_tools common)
