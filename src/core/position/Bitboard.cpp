/* Bitboard.cpp
 *
 * Kubo Ryosuke
 */

#include "core/position/Bitboard.hpp"
#include "common/util/StringUtil.hpp"
#include <sstream>

namespace sunfish {

const std::array<Bitboard, NUMBER_OF_SQUARES> Bitboard::Mask = {{
  Bitboard::zero().set(Square(static_cast<SquareRawType>(0))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(1))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(2))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(3))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(4))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(5))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(6))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(7))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(8))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(9))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(10))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(11))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(12))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(13))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(14))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(15))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(16))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(17))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(18))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(19))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(20))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(21))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(22))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(23))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(24))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(25))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(26))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(27))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(28))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(29))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(30))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(31))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(32))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(33))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(34))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(35))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(36))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(37))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(38))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(39))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(40))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(41))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(42))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(43))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(44))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(45))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(46))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(47))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(48))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(49))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(50))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(51))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(52))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(53))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(54))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(55))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(56))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(57))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(58))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(59))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(60))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(61))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(62))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(63))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(64))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(65))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(66))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(67))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(68))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(69))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(70))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(71))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(72))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(73))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(74))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(75))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(76))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(77))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(78))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(79))),
  Bitboard::zero().set(Square(static_cast<SquareRawType>(80))),
}};

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
