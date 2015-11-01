/* Piece.cpp
 *
 * Kubo Ryosuke
 */

#include "core/base/Piece.hpp"
#include <cstring>
#include <cctype>

namespace {

using namespace sunfish;

const char* namesCsaKindOnly[] = {
  "FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU",
  "TO", "NY", "NK", "NG", "  ", "UM", "RY", "  ",
  "FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU",
  "TO", "NY", "NK", "NG", "  ", "UM", "RY", "  ",
  "  "
};

const char* namesCsa[] = {
  "+FU", "+KY", "+KE", "+GI", "+KI", "+KA", "+HI", "+OU",
  "+TO", "+NY", "+NK", "+NG", "   ", "+UM", "+RY", "   ",
  "-FU", "-KY", "-KE", "-GI", "-KI", "-KA", "-HI", "-OU",
  "-TO", "-NY", "-NK", "-NG", "   ", "-UM", "-RY", "   ",
  "   "
};

}

namespace sunfish {

std::string PieceType::toString() const {
  if (number_ > sizeof(namesCsa) / sizeof(namesCsa[0])) {
    return std::to_string(number_);
  } else {
    return namesCsaKindOnly[number_];
  }
}

PieceType PieceType::parse(const char* str) {
  PIECE_TYPE_EACH(piece) {
    if (strncmp(str, namesCsaKindOnly[piece.number_], 2) == 0) {
      return piece;
    }
  }
  return PieceType::empty();
}

std::string Piece::toString() const {
  if (number_ > sizeof(namesCsa) / sizeof(namesCsa[0])) {
    return std::to_string(number_);
  } else {
    return namesCsa[number_];
  }
}

Piece Piece::parse(const char* str) {
  PIECE_EACH(piece) {
    if (strncmp(str, namesCsa[piece.number_], 3) == 0) {
      return piece;
    }
  }
  return Piece::empty();
}

} // namespace sunfish
