/* Bitboard.cpp
 *
 * Kubo Ryosuke
 */

#include "core/position/Bitboard.hpp"
#include "common/util/StringUtil.hpp"
#include <sstream>

namespace sunfish {

const std::array<Bitboard, NUMBER_OF_SQUARES> Bitboard::Mask = {
  Bitboard().set(Square(static_cast<SquareRawType>(0))),
  Bitboard().set(Square(static_cast<SquareRawType>(1))),
  Bitboard().set(Square(static_cast<SquareRawType>(2))),
  Bitboard().set(Square(static_cast<SquareRawType>(3))),
  Bitboard().set(Square(static_cast<SquareRawType>(4))),
  Bitboard().set(Square(static_cast<SquareRawType>(5))),
  Bitboard().set(Square(static_cast<SquareRawType>(6))),
  Bitboard().set(Square(static_cast<SquareRawType>(7))),
  Bitboard().set(Square(static_cast<SquareRawType>(8))),
  Bitboard().set(Square(static_cast<SquareRawType>(9))),
  Bitboard().set(Square(static_cast<SquareRawType>(10))),
  Bitboard().set(Square(static_cast<SquareRawType>(11))),
  Bitboard().set(Square(static_cast<SquareRawType>(12))),
  Bitboard().set(Square(static_cast<SquareRawType>(13))),
  Bitboard().set(Square(static_cast<SquareRawType>(14))),
  Bitboard().set(Square(static_cast<SquareRawType>(15))),
  Bitboard().set(Square(static_cast<SquareRawType>(16))),
  Bitboard().set(Square(static_cast<SquareRawType>(17))),
  Bitboard().set(Square(static_cast<SquareRawType>(18))),
  Bitboard().set(Square(static_cast<SquareRawType>(19))),
  Bitboard().set(Square(static_cast<SquareRawType>(20))),
  Bitboard().set(Square(static_cast<SquareRawType>(21))),
  Bitboard().set(Square(static_cast<SquareRawType>(22))),
  Bitboard().set(Square(static_cast<SquareRawType>(23))),
  Bitboard().set(Square(static_cast<SquareRawType>(24))),
  Bitboard().set(Square(static_cast<SquareRawType>(25))),
  Bitboard().set(Square(static_cast<SquareRawType>(26))),
  Bitboard().set(Square(static_cast<SquareRawType>(27))),
  Bitboard().set(Square(static_cast<SquareRawType>(28))),
  Bitboard().set(Square(static_cast<SquareRawType>(29))),
  Bitboard().set(Square(static_cast<SquareRawType>(30))),
  Bitboard().set(Square(static_cast<SquareRawType>(31))),
  Bitboard().set(Square(static_cast<SquareRawType>(32))),
  Bitboard().set(Square(static_cast<SquareRawType>(33))),
  Bitboard().set(Square(static_cast<SquareRawType>(34))),
  Bitboard().set(Square(static_cast<SquareRawType>(35))),
  Bitboard().set(Square(static_cast<SquareRawType>(36))),
  Bitboard().set(Square(static_cast<SquareRawType>(37))),
  Bitboard().set(Square(static_cast<SquareRawType>(38))),
  Bitboard().set(Square(static_cast<SquareRawType>(39))),
  Bitboard().set(Square(static_cast<SquareRawType>(40))),
  Bitboard().set(Square(static_cast<SquareRawType>(41))),
  Bitboard().set(Square(static_cast<SquareRawType>(42))),
  Bitboard().set(Square(static_cast<SquareRawType>(43))),
  Bitboard().set(Square(static_cast<SquareRawType>(44))),
  Bitboard().set(Square(static_cast<SquareRawType>(45))),
  Bitboard().set(Square(static_cast<SquareRawType>(46))),
  Bitboard().set(Square(static_cast<SquareRawType>(47))),
  Bitboard().set(Square(static_cast<SquareRawType>(48))),
  Bitboard().set(Square(static_cast<SquareRawType>(49))),
  Bitboard().set(Square(static_cast<SquareRawType>(50))),
  Bitboard().set(Square(static_cast<SquareRawType>(51))),
  Bitboard().set(Square(static_cast<SquareRawType>(52))),
  Bitboard().set(Square(static_cast<SquareRawType>(53))),
  Bitboard().set(Square(static_cast<SquareRawType>(54))),
  Bitboard().set(Square(static_cast<SquareRawType>(55))),
  Bitboard().set(Square(static_cast<SquareRawType>(56))),
  Bitboard().set(Square(static_cast<SquareRawType>(57))),
  Bitboard().set(Square(static_cast<SquareRawType>(58))),
  Bitboard().set(Square(static_cast<SquareRawType>(59))),
  Bitboard().set(Square(static_cast<SquareRawType>(60))),
  Bitboard().set(Square(static_cast<SquareRawType>(61))),
  Bitboard().set(Square(static_cast<SquareRawType>(62))),
  Bitboard().set(Square(static_cast<SquareRawType>(63))),
  Bitboard().set(Square(static_cast<SquareRawType>(64))),
  Bitboard().set(Square(static_cast<SquareRawType>(65))),
  Bitboard().set(Square(static_cast<SquareRawType>(66))),
  Bitboard().set(Square(static_cast<SquareRawType>(67))),
  Bitboard().set(Square(static_cast<SquareRawType>(68))),
  Bitboard().set(Square(static_cast<SquareRawType>(69))),
  Bitboard().set(Square(static_cast<SquareRawType>(70))),
  Bitboard().set(Square(static_cast<SquareRawType>(71))),
  Bitboard().set(Square(static_cast<SquareRawType>(72))),
  Bitboard().set(Square(static_cast<SquareRawType>(73))),
  Bitboard().set(Square(static_cast<SquareRawType>(74))),
  Bitboard().set(Square(static_cast<SquareRawType>(75))),
  Bitboard().set(Square(static_cast<SquareRawType>(76))),
  Bitboard().set(Square(static_cast<SquareRawType>(77))),
  Bitboard().set(Square(static_cast<SquareRawType>(78))),
  Bitboard().set(Square(static_cast<SquareRawType>(79))),
  Bitboard().set(Square(static_cast<SquareRawType>(80))),
};

std::string Bitboard::toString() const {
  std::ostringstream oss;

  for (int rank = 1; rank <= 9; rank++) {
    for (int file = 9; file >= 1; file--) {
      if (check(Square(file, rank))) {
        oss << '1';
      } else {
        oss << '0';
      }
    }
    oss << '\n';
  }

  return oss.str();
}

std::string RotatedBitboard::toString() const {
  return StringUtil::stringify(bb_);
}

}