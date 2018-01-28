cmake_minimum_required(VERSION 2.8)

project(sunfish_csa CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../book "${CMAKE_CURRENT_BINARY_DIR}/book")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_csa
    client/CsaClient.cpp
    client/CsaClient.hpp
    client/Socket.cpp
    client/Socket.hpp
    Main.cpp
)

if(WIN32)
    target_link_libraries(sunfish_csa wsock32 ws2_32)
endif()

target_link_libraries(sunfish_csa search)
target_link_libraries(sunfish_csa book)
target_link_libraries(sunfish_csa core)
target_link_libraries(sunfish_csa common)
target_link_libraries(sunfish_csa logger)
