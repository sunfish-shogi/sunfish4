# Common.cmake

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang|GNU|Intel)")
    # GCC
    if("${PROFILE}" MATCHES "(1|ON)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        set(CMAKE_CXX_FLAGS_RELEASE "-O -DNDEBUG")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -W -msse2 -fno-rtti -pthread")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-exceptions")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX- /MT /fp:precise /arch:SSE2")
    find_library(WSOCK32_LIBRARY wsock32)
endif()

if("${USE_SSE2}" MATCHES "(1|ON)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SSE2=1")
elseif("${USE_SSE2}" MATCHES "(0|OFF)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SSE2=0")
endif()

if("${ENABLE_MEASUREMENT}" MATCHES "(1|ON)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_MEASUREMENT=1")
elseif("${ENABLE_MEASUREMENT}" MATCHES "(0|OFF)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_MEASUREMENT=0")
endif()
