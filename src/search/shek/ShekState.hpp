/* ShekState.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_SHEK_SHEKSTATE_HPP__
#define SUNFISH_SEACH_SHEK_SHEKSTATE_HPP__

#include <iostream>

namespace sunfish {

enum class ShekState : int {
  None,
  Equal,
  Equal4,
  Superior,
  Inferior
};

} // namespace sunfish

inline std::ostream& operator<<(std::ostream& os, sunfish::ShekState shekState) {
  switch (shekState) {
  case sunfish::ShekState::None:
    os << "None";
    break;
  case sunfish::ShekState::Equal:
    os << "Equal";
    break;
  case sunfish::ShekState::Equal4:
    os << "Equal4";
    break;
  case sunfish::ShekState::Superior:
    os << "Superior";
    break;
  case sunfish::ShekState::Inferior:
    os << "Inferior";
    break;
  }
  return os;
}

#endif // SUNFISH_SEACH_SHEK_SHEKSTATE_HPP__
