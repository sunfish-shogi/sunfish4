/* TTSlots.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TT_TTELEMENT_HPP__
#define SUNFISH_TT_TTELEMENT_HPP__

#include "common/Def.hpp"
#include "core/position/Zobrist.hpp"
#include "search/tt/TTElement.hpp"
#include <cstdint>

namespace sunfish {

enum class TTStatus : int {
  None,
  Reject,
  New,
  Update,
  Collide,
};

class TTSlots {
public:

  using SizeType = uint32_t;

  static CONSTEXPR_CONST SizeType Size = 4;

  TTSlots() : lastAccess_(0) {
  }

  TTStatus set(const TTElement& element);

  bool get(Zobrist::Type hash, TTElement& element);

private:

  TTElement slots_[Size];
  volatile SizeType lastAccess_;

};

static_assert(sizeof(TTElement) == 16, "invalid struct size");
static_assert(sizeof(TTSlots) == 72, "invalid struct size");

} // namespace sunfish

#endif // SUNFISH_TT_TTELEMENT_HPP__
