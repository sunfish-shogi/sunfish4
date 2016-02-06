/* Square.cpp
 *
 * Kubo Ryosuke
 */

#include "core/base/Square.hpp"
#include <sstream>

namespace {

using namespace sunfish;

CONSTEXPR_CONST int8_t square2Distance[17][17] = {
  { 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, },
  { 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, },
  { 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, },
  { 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, },
  { 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, },
  { 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, },
  { 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, },
  { 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, },
  { 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, },
  { 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, },
};

enum {
  D00 = static_cast<int8_t>(Direction::None),
  D01 = static_cast<int8_t>(Direction::Up),
  D02 = static_cast<int8_t>(Direction::Down),
  D03 = static_cast<int8_t>(Direction::Left),
  D04 = static_cast<int8_t>(Direction::Right),
  D05 = static_cast<int8_t>(Direction::LeftUp),
  D06 = static_cast<int8_t>(Direction::LeftDown),
  D07 = static_cast<int8_t>(Direction::RightUp),
  D08 = static_cast<int8_t>(Direction::RightDown),
  D09 = static_cast<int8_t>(Direction::LeftUpKnight),
  D10 = static_cast<int8_t>(Direction::LeftDownKnight),
  D11 = static_cast<int8_t>(Direction::RightUpKnight),
  D12 = static_cast<int8_t>(Direction::RightDownKnight),
};

CONSTEXPR_CONST int8_t square2Direction[17][17] = {
  { D05, D00, D00, D00, D00, D00, D00, D00, D01, D00, D00, D00, D00, D00, D00, D00, D07, },
  { D00, D05, D00, D00, D00, D00, D00, D00, D01, D00, D00, D00, D00, D00, D00, D07, D00, },
  { D00, D00, D05, D00, D00, D00, D00, D00, D01, D00, D00, D00, D00, D00, D07, D00, D00, },
  { D00, D00, D00, D05, D00, D00, D00, D00, D01, D00, D00, D00, D00, D07, D00, D00, D00, },
  { D00, D00, D00, D00, D05, D00, D00, D00, D01, D00, D00, D00, D07, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D00, D05, D00, D00, D01, D00, D00, D07, D00, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D00, D00, D05, D09, D01, D11, D07, D00, D00, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D00, D00, D00, D05, D01, D07, D00, D00, D00, D00, D00, D00, D00, },
  { D03, D03, D03, D03, D03, D03, D03, D03, D00, D04, D04, D04, D04, D04, D04, D04, D04, },
  { D00, D00, D00, D00, D00, D00, D00, D06, D02, D08, D00, D00, D00, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D00, D00, D06, D10, D02, D12, D08, D00, D00, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D00, D06, D00, D00, D02, D00, D00, D08, D00, D00, D00, D00, D00, },
  { D00, D00, D00, D00, D06, D00, D00, D00, D02, D00, D00, D00, D08, D00, D00, D00, D00, },
  { D00, D00, D00, D06, D00, D00, D00, D00, D02, D00, D00, D00, D00, D08, D00, D00, D00, },
  { D00, D00, D06, D00, D00, D00, D00, D00, D02, D00, D00, D00, D00, D00, D08, D00, D00, },
  { D00, D06, D00, D00, D00, D00, D00, D00, D02, D00, D00, D00, D00, D00, D00, D08, D00, },
  { D06, D00, D00, D00, D00, D00, D00, D00, D02, D00, D00, D00, D00, D00, D00, D00, D08, },
};

}

namespace sunfish {

const uint8_t Square::rotate90Table[81] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   2,  9, 16, 23, 30, 37, 44, 51, 58,
   3, 10, 17, 24, 31, 38, 45, 52, 59,
   4, 11, 18, 25, 32, 39, 46, 53, 60,
   5, 12, 19, 26, 33, 40, 47, 54, 61,
   6, 13, 20, 27, 34, 41, 48, 55, 62,
   7, 14, 21, 28, 35, 42, 49, 56, 63,
   0,  0,  0,  0,  0,  0,  0,  0,  0,
};

const uint8_t Square::rotateRight45Table[81] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  1,  2,  4,  7, 11, 16, 22,  0,
   0,  3,  5,  8, 12, 17, 23, 29,  0,
   0,  6,  9, 13, 18, 24, 30, 35,  0,
   0, 10, 14, 19, 25, 31, 36, 40,  0,
   0, 15, 20, 26, 32, 37, 41, 44,  0,
   0, 21, 27, 33, 38, 42, 45, 47,  0,
   0, 28, 34, 39, 43, 46, 48, 49,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,
};

const uint8_t Square::rotateLeft45Table[81] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,
   0, 22, 16, 11,  7,  4,  2,  1,  0,
   0, 29, 23, 17, 12,  8,  5,  3,  0,
   0, 35, 30, 24, 18, 13,  9,  6,  0,
   0, 40, 36, 31, 25, 19, 14, 10,  0,
   0, 44, 41, 37, 32, 26, 20, 15,  0,
   0, 47, 45, 42, 38, 33, 27, 21,  0,
   0, 49, 48, 46, 43, 39, 34, 28,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,
};

SquareRawType Square::distance(const Square& to) const {
  int x = (int)to.number_ % RankMax - (int)number_ % RankMax + 8;
  int y = (int)to.number_ / RankMax - (int)number_ / RankMax + 8;
  return square2Distance[x][y];
}

Direction Square::dir(const Square& to) const {
  int x = (int)to.number_ % RankMax - (int)number_ % RankMax + 8;
  int y = (int)to.number_ / RankMax - (int)number_ / RankMax + 8;
  return static_cast<Direction>(square2Direction[x][y]);
}

std::string Square::toString() const {
  std::ostringstream oss;
  oss << (int)getFile() << (int)getRank();
  return oss.str();
}

Square Square::parse(const char* str) {
  if (str[0] >= '1' && str[0] <= '9' && str[1] >= '1' && str[1] <= '9') {
    return Square(str[0] - '0', str[1] - '0');
  }
  return Square::invalid();
}

std::string Square::toStringSFEN() const {
  std::ostringstream oss;
  oss << (int)getFile() << (char)(('a' - 1) + getRank());
  return oss.str();
}

Square Square::parseSFEN(const char* str) {
  if (str[0] >= '1' && str[0] <= '9' && str[1] >= 'a' && str[1] <= 'i') {
    return Square(str[0] - '0', str[1] - ('a' - 1));
  }
  return Square::invalid();
}

std::string RelativeSquare::toString() const {
  std::ostringstream oss;
  oss << '[' << (getFile() - 8) << ',' << (getRank() - 8) << ']';
  return oss.str();
}

} // namespace sunfish
