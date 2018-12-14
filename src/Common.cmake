# Common.cmake

if(WIN32)
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_MINSIZEREL       "${CMAKE_C_FLAGS_MINSIZEREL}")
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE}")
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
else()
    if("${PROFILE}" MATCHES "(1|ON)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_ROOT_MOVES_SHUFFLE=1")
        set(CMAKE_CXX_FLAGS_RELEASE "-O -DNDEBUG")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -W -msse2 -fno-rtti -pthread")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-exceptions")
endif()

if("${LEARNING}" MATCHES "(1|ON)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLEARNING=1")
elseif("${LEARNING}" MATCHES "(0|OFF)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLEARNING=0")
endif()

if("${MATERIAL_LEARNING}" MATCHES "(1|ON)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATERIAL_LEARNING=1")
elseif("${MATERIAL_LEARNING}" MATCHES "(0|OFF)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATERIAL_LEARNING=0")
endif()

if("${MATERIAL_LEARNING_ONLY}" MATCHES "(1|ON)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATERIAL_LEARNING_ONLY=1")
elseif("${MATERIAL_LEARNING_ONLY}" MATCHES "(0|OFF)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATERIAL_LEARNING_ONLY=0")
endif()
