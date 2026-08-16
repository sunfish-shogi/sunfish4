cmake_minimum_required(VERSION 3.13)
project(sunfish4_wasm CXX)

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Build this target with emcmake")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
include(../Common.cmake)
include_directories("..")

add_subdirectory(../search "${CMAKE_CURRENT_BINARY_DIR}/search")
add_subdirectory(../core "${CMAKE_CURRENT_BINARY_DIR}/core")
add_subdirectory(../common "${CMAKE_CURRENT_BINARY_DIR}/common")
add_subdirectory(../logger "${CMAKE_CURRENT_BINARY_DIR}/logger")

add_executable(sunfish4 Exports.cpp WasmUsiClient.cpp WasmUsiClient.hpp)
target_link_libraries(sunfish4 search core common logger)
set_target_properties(sunfish4 PROPERTIES SUFFIX ".js")

target_link_options(sunfish4 PRIVATE
  "-sMODULARIZE=1"
  "-sEXPORT_ES6=1"
  "-sEXPORT_NAME=createSunfish4"
  "-sENVIRONMENT=worker,node"
  "-sINVOKE_RUN=0"
  "-sALLOW_MEMORY_GROWTH=1"
  "-sSTACK_SIZE=1048576"
  "--no-entry"
  "--pre-js=${CMAKE_CURRENT_SOURCE_DIR}/shim.js"
  "-sEXPORTED_FUNCTIONS=_usi_command,_usi_poll,_malloc,_free"
  "-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS"
)
