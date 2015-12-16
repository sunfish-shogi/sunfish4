cmake_minimum_required(VERSION 2.8)

project(sunfish_test CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories("..")

add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_test
    Main.cpp
    core/BitboardTest.cpp
    core/CsaReaderTest.cpp
    core/MoveGeneratorTest.cpp
    core/MovesTest.cpp
    core/MoveTablesTest.cpp
    core/MoveTest.cpp
    core/PieceTest.cpp
    core/PositionTest.cpp
    core/SfenParserTest.cpp
    core/SquareTest.cpp
    search/MaterialTest.cpp
    search/ScoreTest.cpp
)

target_link_libraries(sunfish_test core)
target_link_libraries(sunfish_test search)
target_link_libraries(sunfish_test logger)
