/* TTSlots.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTSlots.hpp"

namespace sunfish {

TTStatus TTSlots::set(const TTElement& element) {
  const SizeType lastAccess = lastAccess_;

  static_assert(Size == 3, "this logic is valid only when the slot size is 3.");

  // search a slot which has a same hash value.
  SizeType curr = lastAccess;
  if (slots_[curr].hash() == element.hash()) {
    slots_[curr] = element;
    return TTStatus::Update;
  }

  SizeType prev = (lastAccess + 2) % Size;
  if (slots_[prev].hash() == element.hash()) {
    slots_[prev] =  element;
    lastAccess_ = prev;
    return TTStatus::Update;
  }

  SizeType prev2 = (lastAccess + 1) % Size;
  if (slots_[prev2].hash() == element.hash()) {
    slots_[prev2] = element;
    lastAccess_ = prev2;
    return TTStatus::Update;
  }

  // find a vacant slot
  for (SizeType i = 0; i < Size; i++) {
    SizeType index = (lastAccess + 1 + i) % Size;
    if (!slots_[index].isLive()) {
      slots_[index] = element;
      lastAccess_ = index;
      return TTStatus::New;
    }
  }

  // overwrite
  slots_[prev2] = element;
  lastAccess_ = prev2;
  return TTStatus::Collide;
}

bool TTSlots::get(Zobrist::Type hash, TTElement& element) {
  const SizeType lastAccess = lastAccess_;

  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    const SizeType index = (lastAccess + Size - i) % Size;
    TTElement e = slots_[index];
    if (e.checkHash(hash)) {
      element = e;
      return true;
    }
  }
  return false;

}

unsigned TTSlots::fullCount() const {
  unsigned count = 0;
  for (SizeType i = 0; i < Size; i++) {
    if (slots_[i].isLive()) {
      count++;
    }
  }
  return count;
}

} // namespace sunfish
