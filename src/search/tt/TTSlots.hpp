/* TTSlots.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TT_TTELEMENT_HPP__
#define SUNFISH_TT_TTELEMENT_HPP__

#include "common/Def.hpp"
#include "core/position/Zobrist.hpp"
#include "search/tt/TTElement.hpp"
#include <cassert>

namespace sunfish {

class TTSlots {
public:

  using SizeType = uint32_t;

  static CONSTEXPR_CONST SizeType Size = 4;

  TTSlots() : lastAccess_(0) {
  }

  void clear(SizeType) {
    for (SizeType i = 0; i < Size; i++) {
      slots_[i].clear();
    }
  }

  TTStatus set(const TTElement& entity);

  bool get(Zobrist::Type hash, TTElement& entity);

private:

  TTElement slots_[Size];
  volatile SizeType lastAccess_;

};

static_assert(sizeof(TTElement) == 16, "invalid struct size");
static_assert(sizeof(TTSlots) == 72, "invalid struct size");

} // namespace sunfish

#endif // SUNFISH_TT_TTELEMENT_HPP__
