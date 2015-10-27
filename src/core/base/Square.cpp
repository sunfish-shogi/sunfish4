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

int32_t Square::distance(const Square& to) const {
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
