cmake_minimum_required(VERSION 2.8)

project(sunfish_expt CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")

add_executable(sunfish_expt
    Main.cpp
    mgtest/MoveGenerationTest.cpp
    mgtest/MoveGenerationTest.hpp
    mgtest/TardyMoveGenerator.cpp
    mgtest/TardyMoveGenerator.hpp
    solve/Solver.cpp
    solve/Solver.hpp
)

target_link_libraries(sunfish_expt search)
target_link_libraries(sunfish_expt core)
target_link_libraries(sunfish_expt logger)
target_link_libraries(sunfish_expt common)
