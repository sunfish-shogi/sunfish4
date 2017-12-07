cmake_minimum_required(VERSION 2.8)

project(sunfish_ln CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(../Common.cmake)

include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish_ln
    batch/BatchLearning.cpp
    batch/BatchLearning.hpp
    gradient/Gradient.cpp
    gradient/Gradient.hpp
    online/OnlineLearning.cpp
    online/OnlineLearning.hpp
    util/LearningUtil.cpp
    util/LearningUtil.hpp
    Main.cpp
)

target_link_libraries(sunfish_ln search)
target_link_libraries(sunfish_ln core)
target_link_libraries(sunfish_ln common)
target_link_libraries(sunfish_ln logger)
