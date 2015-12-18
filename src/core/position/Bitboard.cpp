/* Bitboard.cpp
 *
 * Kubo Ryosuke
 */

#include "core/position/Bitboard.hpp"
#include "common/string/StringUtil.hpp"
#include <sstream>

namespace sunfish {

std::array<Bitboard, NUMBER_OF_SQUARES> Bitboard::Mask;
std::array<std::array<Bitboard, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> Bitboard::LineMask;

void Bitboard::initialize() {
  SQUARE_EACH(square) {
    Mask[square.raw()] = Bitboard::zero().set(square);
  }

  SQUARE_EACH(from) {
    SQUARE_EACH(to) {
      LineMask[from.raw()][to.raw()] = Bitboard::zero();

      Direction dir = from.dir(to);
      if (dir == Direction::None) {
        continue;
      }

      for (auto square = from.move(dir); ; square = square.move(dir)) {
        LineMask[from.raw()][to.raw()].set(square);
        if (square == to) {
          break;
        }
      }
    }
  }
}

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
