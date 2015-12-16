/* TTSlots.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTSlots.hpp"

namespace sunfish {

TTStatus TTSlots::set(const TTElement& element) {
  const SizeType lastAccess = lastAccess_;

  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess + i) % Size;
    if (slots_[index].hash() == element.hash()) {
      slots_[index] = element;
      lastAccess_ = index;
      return TTStatus::Update;
    }
  }

  // find a vacant slot
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess + 1 + i) % Size;
    if (slots_[index].age() != element.age()) {
      slots_[index] = element;
      lastAccess_ = index;
      return TTStatus::New;
    }
  }

  // overwrite
  const SizeType index = (lastAccess + 1) % Size;
  slots_[index] = element;
  lastAccess_ = index;
  return TTStatus::Collide;

}

bool TTSlots::get(Zobrist::Type hash, TTElement& element) {
  const SizeType lastAccess = lastAccess_;

  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess + i) % Size;
    TTElement e = slots_[index];
    if (e.checkHash(hash)) {
      element = e;
      lastAccess_ = index;
      return true;
    }
  }
  return false;

}

} // namespace sunfish
