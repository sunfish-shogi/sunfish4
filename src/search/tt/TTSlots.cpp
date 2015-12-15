/* TTSlots.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTSlots.hpp"

namespace sunfish {

TTStatus TTSlots::set(const TTElement& entity) {
  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess_ + i) % Size;
    if (slots_[index].hash() == entity.hash()) {
      slots_[index] = entity;
      lastAccess_ = index;
      return TTStatus::Update;
    }
  }

  // find a vacant slot
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess_ + 1 + i) % Size;
    if (slots_[index].age() != entity.age()) {
      slots_[index] = entity;
      lastAccess_ = index;
      return TTStatus::New;
    }
  }

  // overwrite
  const SizeType index = (lastAccess_ + 1) % Size;
  slots_[index] = entity;
  lastAccess_ = index;
  return TTStatus::Collide;

}

bool TTSlots::get(Zobrist::Type hash, TTElement& entity) {
  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess_ + i) % Size;
    if (slots_[index].checkHash(hash)) {
      entity = slots_[index];
      lastAccess_ = index;
      return true;
    }
  }
  return false;

}

} // namespace sunfish
