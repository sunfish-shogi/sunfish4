cmake_minimum_required(VERSION 2.8)

add_library(common STATIC
    bitope/BitOpe.hpp
    console/Console.hpp
    Def.hpp
    file_system/Directory.cpp
    file_system/Directory.hpp
    file_system/FileUtil.cpp
    file_system/FileUtil.hpp
    math/Random.hpp
    memory/Memory.hpp
    program_options/ProgramOptions.hpp
    resource/Resource.cpp
    resource/Resource.hpp
    string/StringUtil.hpp
    string/TablePrinter.hpp
    string/Wildcard.cpp
    string/Wildcard.hpp
    thread/ScopedThread.hpp
    time/Timer.hpp
)
