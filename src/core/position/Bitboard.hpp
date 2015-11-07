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
#include <array>
#include <cstdint>

#define BB_FILES_1ST 5
#define BB_FILES_2ND 4

#define BB_SQUARES_1ST (BB_FILES_1ST*SQUARE_RANKS)
#define BB_SQUARES_2ND (BB_FILES_2ND*SQUARE_RANKS)

namespace sunfish {

class Bitboard : public Bitset128<Bitboard, SquareRawType, BB_SQUARES_1ST, BB_SQUARES_2ND> {
public:

  using Bitset128::Bitset128;

  Bitboard& set(const Square& square) {
    return Bitset128::set(square.raw());
  }

  Bitboard& unset(const Square& square) {
    return Bitset128::unset(square.raw());
  }

  bool check(const Square& square) const {
    return Bitset128::check(square.raw());
  }

  static const Bitboard& mask(const Square& square) {
    return Mask[square.raw()];
  }

private:

  static const std::array<Bitboard, NUMBER_OF_SQUARES> Mask;

};

class RotatedBitboard : public Bitset64<RotatedBitboard, SquareRawType> {
public:

  using Bitset64::Bitset64;

  RotatedBitboard& set(const RotatedSquare& square) {
    return Bitset64::set(square.raw());
  }

  RotatedBitboard& unset(const RotatedSquare& square) {
    return Bitset64::unset(square.raw());
  }

  bool check(const RotatedSquare& square) const {
    return Bitset64::check(square.raw());
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITBOARD_HPP__
