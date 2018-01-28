/* Turn.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_TURN_HPP__
#define SUNFISH_CORE_BASE_TURN_HPP__

#include <cstdint>
#include <iostream>

namespace sunfish {

enum Turn : bool {
  Black = true,
  White = false,
};

inline const char* turnToString(Turn turn) {
  return (turn == sunfish::Turn::Black ? "Black" : "White");
}

}

inline std::ostream& operator<<(std::ostream& os, sunfish::Turn turn) {
  os << turnToString(turn);
  return os;
}

#endif // SUNFISH_CORE_BASE_TURN_HPP__
