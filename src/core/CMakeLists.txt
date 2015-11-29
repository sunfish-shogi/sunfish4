cmake_minimum_required(VERSION 2.8)

project(sunfish CXX)

add_library(core STATIC
	base/Piece.cpp
	base/Square.cpp
	move/Move.cpp
	move/MoveGenerator.cpp
	move/MoveTables.cpp
	position/Bitboard.cpp
	position/Position.cpp
	position/Zobrist.cpp
	record/CsaReader.cpp
    record/SfenParser.cpp
)
