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

class Bitboard : public Bitset128<Bitboard, SquareRawType, BB_SQUARES_1ST, BB_SQUARES_2ND, Square::Invalid> {
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

  static Bitboard blackPromotable() {
    return Bitboard(0x00000070381c0e07, 0x00000000381c0e07);
  }

  static Bitboard whitePromotable() {
    return Bitboard(0x00001c0e070381c0, 0x0000000e070381c0);
  }

  static Bitboard blackPromotable2() {
    return Bitboard(0x00000030180c0603, 0x00000000180c0603);
  }

  static Bitboard whitePromotable2() {
    return Bitboard(0x0000180c06030180, 0x0000000c06030180);
  }

  static Bitboard blackNotPromotable() {
    return Bitboard(0x00001f8fc7e3f1f8, 0x0000000fc7e3f1f8);
  }

  static Bitboard whiteNotPromotable() {
    return Bitboard(0x000003f1f8fc7e3f, 0x00000001f8fc7e3f);
  }

  std::string toString() const;

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

  std::string toString() const;

};

} // namespace sunfish

inline std::ostream& operator<<(std::ostream& os, const sunfish::Bitboard& bb) {
  os << bb.toString();
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::RotatedBitboard& bb) {
  os << bb.toString();
  return os;
}

#define BB_EACH(square, bb) for (sunfish::Square square(bb.pickFirst()); square.isValid(); square = Square(bb.pickFirst()))

#endif // SUNFISH_CORE_POSITION_BITBOARD_HPP__
