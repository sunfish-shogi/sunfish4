cmake_minimum_required(VERSION 2.8)

project(sunfish_test CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../book "${CMAKE_CURRENT_BINARY_DIR}/book")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_test
    Main.cpp
    book/BookTest.cpp
    book/BookGeneratorTest.cpp
    core/BitboardTest.cpp
    core/CsaReaderTest.cpp
    core/CsaWriterTest.cpp
    core/MoveGeneratorTest.cpp
    core/MovesTest.cpp
    core/MoveTablesTest.cpp
    core/MoveTest.cpp
    core/PieceTest.cpp
    core/PositionTest.cpp
    core/SfenParserTest.cpp
    core/SquareTest.cpp
    search/EvaluatorTest.cpp
    search/FeatureVectorTest.cpp
    search/MaterialTest.cpp
    search/MateTest.cpp
    search/ScoreTest.cpp
    search/SCRDetectorTest.cpp
    search/SEETest.cpp
    search/ShekTest.cpp
    search/TTTest.cpp
)

target_link_libraries(sunfish_test search)
target_link_libraries(sunfish_test book)
target_link_libraries(sunfish_test core)
target_link_libraries(sunfish_test common)
target_link_libraries(sunfish_test logger)
