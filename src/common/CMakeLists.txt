cmake_minimum_required(VERSION 2.8)

add_library(common STATIC
	file_system/Directory.cpp
    file_system/FileUtil.cpp
    resource/Resource.cpp
    string/Wildcard.cpp
)
