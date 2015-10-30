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

class RotatedBitboard : public Bitset128<RotatedBitboard, 64, 64> {
public:

  using Bitset128::Bitset128;

  void set1(const RotatedSquare& square) {
    Bitset128::set1(square.raw());
  }

  void unset1(const RotatedSquare& square) {
    Bitset128::unset1(square.raw());
  }

  bool check1(const RotatedSquare& square) const {
    return Bitset128::check1(square.raw());
  }

  void set2(const RotatedSquare& square) {
    Bitset128::set2(square.raw());
  }

  void unset2(const RotatedSquare& square) {
    Bitset128::unset2(square.raw());
  }

  bool check2(const RotatedSquare& square) const {
    return Bitset128::check2(square.raw());
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_BASE_BITBOARD_HPP__
