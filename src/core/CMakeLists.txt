cmake_minimum_required(VERSION 2.8)

add_library(core STATIC
    base/Piece.cpp
    base/Piece.hpp
    base/Square.cpp
    base/Square.hpp
    base/Turn.hpp
    move/Move.cpp
    move/Move.hpp
    move/MoveGenerator.cpp
    move/MoveGenerator.hpp
    move/Moves.cpp
    move/Moves.hpp
    move/MoveTables.cpp
    move/MoveTables.hpp
    position/Bitboard.cpp
    position/Bitboard.hpp
    position/Bitset128.hpp
    position/Bitset64.hpp
    position/Hand.hpp
    position/Position.cpp
    position/Position.hpp
    position/Zobrist.cpp
    position/Zobrist.hpp
    record/CsaReader.cpp
    record/CsaReader.hpp
    record/CsaWriter.cpp
    record/CsaWriter.hpp
    record/Record.cpp
    record/Record.hpp
    record/SfenParser.cpp
    record/SfenParser.hpp
    util/CoreUtil.hpp
    util/PositionUtil.hpp
)
