cmake_minimum_required(VERSION 2.8)

project(sunfish CXX)

add_library(core STATIC
	base/Piece.cpp
	base/Square.cpp
	position/Position.cpp
)
