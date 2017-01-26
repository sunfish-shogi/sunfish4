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

  using SizeType = uint16_t;

  static CONSTEXPR_CONST SizeType Size = 3;

  TTSlots() : lastAccess_(0) {
  }

  TTStatus set(const TTElement& element);

  bool get(Zobrist::Type hash, TTElement& element);

  unsigned fullCount() const;

private:

  TTElement slots_[Size];
  volatile SizeType lastAccess_;

};

static_assert(sizeof(TTElement) == 10, "invalid struct size");
static_assert(sizeof(TTSlots) <= 32, "invalid struct size");

} // namespace sunfish

#endif // SUNFISH_TT_TTELEMENT_HPP__
