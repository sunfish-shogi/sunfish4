cmake_minimum_required(VERSION 2.8)

project(sunfish_bm CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories("..")

add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_bm
    Main.cpp
    core/MoveGeneratorBM.cpp
    core/PositionBM.cpp
    search/EvaluatorBM.cpp
)

target_link_libraries(sunfish_bm core)
target_link_libraries(sunfish_bm search)
target_link_libraries(sunfish_bm logger)
