cmake_minimum_required(VERSION 2.8)

project(sunfish CXX)

add_library(core STATIC
	move/Move.cpp
	base/Piece.cpp
	base/Square.cpp
	position/Position.cpp
	position/Zobrist.cpp
	record/CsaReader.cpp
)
