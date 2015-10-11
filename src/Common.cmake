# Common.cmake

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang|GNU|Intel)")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -W -msse2 -fno-rtti -pthread")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG -fno-exceptions")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX- /nologo /D WIN32 /D _CONSOLE /Oy- /Gm- /Gd /GS- /MT /fp:precise /arch:SSE2")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /Ot /D NDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Zi /Od /D")
	find_library(WSOCK32_LIBRARY wsock32)
endif()
