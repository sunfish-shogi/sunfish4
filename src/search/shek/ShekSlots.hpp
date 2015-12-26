/* ShekSlots.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_SHEK_SHEKSLOTS_HPP__
#define SUNFISH_SEACH_SHEK_SHEKSLOTS_HPP__

#include "common/Def.hpp"
#include "search/shek/ShekElement.hpp"
#include <cstdint>

using SizeType = uint32_t;

namespace sunfish {

class ShekSlots {
public:

  static CONSTEXPR_CONST SizeType Size = 4;

  ShekElement* findSlot(Zobrist::Type hash) {
    for (SizeType i = 0; i < Size; i++) {
      if (slots_[i].checkHash(hash)) {
        return &slots_[i];
      }
    }
    return nullptr;
  }

  ShekElement* findSlot(Zobrist::Type hash, const HandSet& handSet) {
    for (SizeType i = 0; i < Size; i++) {
      if (slots_[i].checkHash(hash) && slots_[i].handSet() == handSet) {
        return &slots_[i];
      }
    }
    return nullptr;
  }

  ShekElement* findVacantSlot() {
    for (SizeType i = 0; i < Size; i++) {
      if (slots_[i].isVacant()) {
        return &slots_[i];
      }
    }
    return nullptr;
  }

private:

  ShekElement slots_[Size];

};

} // namespace sunfish

#endif // SUNFISH_SEACH_SHEK_SHEKSLOTS_HPP__
