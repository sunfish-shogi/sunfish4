cmake_minimum_required(VERSION 2.8)

add_library(core STATIC
    base/Piece.cpp
    base/Square.cpp
    move/Move.cpp
    move/MoveGenerator.cpp
    move/Moves.cpp
    move/MoveTables.cpp
    position/Bitboard.cpp
    position/Position.cpp
    position/Zobrist.cpp
    record/CsaReader.cpp
    record/CsaWriter.cpp
    record/Record.cpp
    record/SfenParser.cpp
)
