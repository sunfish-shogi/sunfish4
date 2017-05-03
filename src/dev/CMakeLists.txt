cmake_minimum_required(VERSION 2.8)

project(sunfish_dev CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_dev
    Main.cpp
    code_generator/ZobristCodeGenerator.cpp
    conv_eval/ConvertEvalBin.cpp
)

target_link_libraries(sunfish_dev search)
target_link_libraries(sunfish_dev core)
target_link_libraries(sunfish_dev logger)
