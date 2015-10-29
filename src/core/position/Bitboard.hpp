/* Bitboard.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_BITBOARD_HPP__
#define SUNFISH_CORE_BASE_BITBOARD_HPP__

#include "common/Def.hpp"
#include "core/base/Square.hpp"
#include "core/position/Bitset128.hpp"
#include <cstdint>

#define BB_FILES_1ST 5
#define BB_FILES_2ND 4

#define BB_SQUARES_1ST (BB_FILES_1ST*SQUARE_RANKS)
#define BB_SQUARES_2ND (BB_FILES_2ND*SQUARE_RANKS)

#define BB_RANKS_1ST 5
#define BB_RANKS_2ND 4

#define BB_R90_SQUARES_1ST (BB_RANKS_1ST*SQUARE_FILES)
#define BB_R90_SQUARES_2ND (BB_RANKS_2ND*SQUARE_FILES)

#define BB_R45_SQUARES_1ST 50
#define BB_R45_SQUARES_2ND 50

namespace sunfish {

class Bitboard : public Bitset128<Bitboard, BB_SQUARES_1ST, BB_SQUARES_2ND> {
public:

  using Bitset128::Bitset128;

  void set(const Square& square) {
    Bitset128::set(square.raw());
  }

  void unset(const Square& square) {
    Bitset128::unset(square.raw());
  }

  bool check(const Square& square) const {
    return Bitset128::check(square.raw());
  }

};

class Rotated90Bitboard : public Bitset128<Bitboard, BB_R90_SQUARES_1ST, BB_R90_SQUARES_2ND> {
public:

  using Bitset128::Bitset128;

  void set(const RotatedSquare& square) {
    Bitset128::set(square.raw());
  }

  void unset(const RotatedSquare& square) {
    Bitset128::unset(square.raw());
  }

  bool check(const RotatedSquare& square) const {
    return Bitset128::check(square.raw());
  }

};

class Rotated45Bitboard : public Bitset128<Bitboard, BB_R45_SQUARES_1ST, BB_R45_SQUARES_2ND> {
public:

  using Bitset128::Bitset128;

  void set(const RotatedSquare& square) {
    Bitset128::set(square.raw());
  }

  void unset(const RotatedSquare& square) {
    Bitset128::unset(square.raw());
  }

  bool check(const RotatedSquare& square) const {
    return Bitset128::check(square.raw());
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_BASE_BITBOARD_HPP__
