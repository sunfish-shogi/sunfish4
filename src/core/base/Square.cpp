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

const uint8_t Square::rotate0Table[81] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,
   1,  2,  3,  4,  5,  6,  7,  8,  9,
  10, 11, 12, 13, 14, 15, 16, 17, 18,
  19, 20, 21, 22, 23, 24, 25, 26, 27,
  28, 29, 30, 31, 32, 33, 34, 35, 36,
  37, 38, 39, 40, 41, 42, 43, 44, 45,
  46, 47, 48, 49, 50, 51, 52, 53, 54,
  55, 56, 57, 58, 59, 60, 61, 62, 63, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,
};

const uint8_t Square::rotate90Table[81] = {
   0,  1, 10, 19, 28, 37, 46, 55,  0,
   0,  2, 11, 20, 29, 38, 47, 56,  0,
   0,  3, 12, 21, 30, 39, 48, 57,  0,
   0,  4, 13, 22, 31, 40, 49, 58,  0,
   0,  5, 14, 23, 32, 41, 50, 59,  0,
   0,  6, 15, 24, 33, 42, 51, 60,  0,
   0,  7, 16, 25, 34, 43, 52, 61,  0,
   0,  8, 17, 26, 35, 44, 53, 62,  0,
   0,  9, 18, 27, 36, 45, 54, 63,  0,
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

Square::RawType Square::distance(const Square& to) const {
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
  return Square::Invalid;
}

} // namespace sunfish
