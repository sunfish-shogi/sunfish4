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

  static void initialize();

  Bitboard& set(const Square& square) {
    return Bitset128::set(square.raw());
  }

  Bitboard& unset(const Square& square) {
    return Bitset128::unset(square.raw());
  }

  bool check(const Square& square) const {
    return Bitset128::check(square.raw());
  }

  bool checkFile(SquareRawType file) const {
    SquareRawType rfile = 9 - file;
    SquareRawType offset = rfile * 9;
    if (offset < Width1) {
      return (first() >> offset) & 0x1ff;
    } else {
      return (second() >> (offset - Width1)) & 0x1ff;
    }
  }

  static const Bitboard& mask(const Square& square) {
    return Mask[square.raw()];
  }

  static const Bitboard& mask(const Square& from, const Square& to) {
    return LineMask[from.raw()][to.raw()];
  }

  static CONSTEXPR Bitboard full() {
    return Bitboard(Mask1, Mask2);
  }

  static CONSTEXPR Bitboard blackPromotable() {
    return Bitboard(0x00000070381c0e07, 0x00000000381c0e07);
  }

  static CONSTEXPR Bitboard whitePromotable() {
    return Bitboard(0x00001c0e070381c0, 0x0000000e070381c0);
  }

  static CONSTEXPR Bitboard blackPromotable2() {
    return Bitboard(0x00000030180c0603, 0x00000000180c0603);
  }

  static CONSTEXPR Bitboard whitePromotable2() {
    return Bitboard(0x0000180c06030180, 0x0000000c06030180);
  }

  static CONSTEXPR Bitboard blackNotPromotable() {
    return Bitboard(0x00001f8fc7e3f1f8, 0x0000000fc7e3f1f8);
  }

  static CONSTEXPR Bitboard whiteNotPromotable() {
    return Bitboard(0x000003f1f8fc7e3f, 0x00000001f8fc7e3f);
  }

  static CONSTEXPR Bitboard rank1() {
    return Bitboard(0x0000001008040201, 0x0000000008040201);
  }

  static CONSTEXPR Bitboard rank2() {
    return Bitboard(0x0000002010080402, 0x0000000010080402);
  }

  static CONSTEXPR Bitboard rank3to9() {
    return Bitboard(0x00001fcfe7f3f9fc, 0x0000000fe7f3f9fc);
  }

  static CONSTEXPR Bitboard rank2to9() {
    return Bitboard(0x00001feff7fbfdfe, 0x0000000ff7fbfdfe);
  }

  static CONSTEXPR Bitboard rank9() {
    return Bitboard(0x0000100804020100, 0x0000000804020100);
  }

  static CONSTEXPR Bitboard rank8() {
    return Bitboard(0x0000080402010080, 0x0000000402010080);
  }

  static CONSTEXPR Bitboard rank1to7() {
    return Bitboard(0x000007f3f9fcfe7f, 0x00000003f9fcfe7f);
  }

  static CONSTEXPR Bitboard rank1to8() {
    return Bitboard(0x00000ff7fbfdfeff, 0x00000007fbfdfeff);
  }

  static CONSTEXPR Bitboard file1() {
    return Bitboard(0x0000000000000000, 0x0000000ff8000000);
  }

  static CONSTEXPR Bitboard file2() {
    return Bitboard(0x0000000000000000, 0x0000000007fc0000);
  }

  static CONSTEXPR Bitboard file3() {
    return Bitboard(0x0000000000000000, 0x000000000003fe00);
  }

  static CONSTEXPR Bitboard file4() {
    return Bitboard(0x0000000000000000, 0x00000000000001ff);
  }

  static CONSTEXPR Bitboard file5() {
    return Bitboard(0x00001ff000000000, 0x0000000000000000);
  }

  static CONSTEXPR Bitboard file6() {
    return Bitboard(0x0000000ff8000000, 0x0000000000000000);
  }

  static CONSTEXPR Bitboard file7() {
    return Bitboard(0x0000000007fc0000, 0x0000000000000000);
  }

  static CONSTEXPR Bitboard file8() {
    return Bitboard(0x000000000003fe00, 0x0000000000000000);
  }

  static CONSTEXPR Bitboard file9() {
    return Bitboard(0x00000000000001ff, 0x0000000000000000);
  }

  static CONSTEXPR Bitboard nocorner() {
    return Bitboard(0x00001ffffffffefe, 0x00000007f7ffffff);
  }

  std::string toString() const;

  friend std::ostream& operator<<(std::ostream& os, const sunfish::Bitboard& bb) {
    os << bb.toString();
    return os;
  }

private:

  static std::array<Bitboard, NUMBER_OF_SQUARES> Mask;
  static std::array<std::array<Bitboard, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> LineMask;

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

  friend std::ostream& operator<<(std::ostream& os, const sunfish::RotatedBitboard& bb) {
    os << bb.toString();
    return os;
  }

};

} // namespace sunfish

#define BB_EACH(square, bb) for (sunfish::Square square(bb.pickFirst()); square.isValid(); square = Square(bb.pickFirst()))

#endif // SUNFISH_CORE_POSITION_BITBOARD_HPP__
