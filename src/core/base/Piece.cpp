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

std::string Piece::toString(bool kind_only) const {
  if (index_ > sizeof(namesCsa) / sizeof(namesCsa[0])) {
    return std::to_string(index_);
  } else if (kind_only) {
    return namesCsaKindOnly[index_];
  } else {
    return namesCsa[index_];
  }
}

Piece Piece::parse(const char* str) {
  if (str[0] == '+' || str[0] == '-') {
    PIECE_EACH(piece) {
      if (strncmp(str, namesCsa[piece.index_], 3) == 0) {
        return piece;
      }
    }
  } else if (isalpha(str[0])) {
    PIECE_KIND_EACH(piece) {
      if (strncmp(str, namesCsaKindOnly[piece.index_], 2) == 0) {
        return piece;
      }
    }
  }
  return Piece::Empty;
}

} // namespace sunfish
