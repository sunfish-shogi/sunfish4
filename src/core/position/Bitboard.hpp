/* Bitboard.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_BITBOARD_HPP__
#define SUNFISH_CORE_POSITION_BITBOARD_HPP__

#include "common/Def.hpp"
#include "core/base/Square.hpp"
#include "core/position/Bitset128.hpp"
#include "core/position/Bitset64.hpp"
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

class RotatedBitboard : public Bitset64<RotatedBitboard> {
public:

  using Bitset64::Bitset64;

  void set(const RotatedSquare& square) {
    Bitset64::set(square.raw());
  }

  void unset(const RotatedSquare& square) {
    Bitset64::unset(square.raw());
  }

  bool check(const RotatedSquare& square) const {
    return Bitset64::check(square.raw());
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITBOARD_HPP__
