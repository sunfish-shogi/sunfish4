/* TTSlots.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTSlots.hpp"

namespace sunfish {

TTStatus TTSlots::set(const TTElement& element) {
  const SizeType lastAccess = lastAccess_;

  static_assert(Size == 4, "this logic is valid only when the slot size is 4.");

  // search a slot which has a same hash value.
  SizeType curr = lastAccess;
  if (slots_[curr].hash() == element.hash()) {
    slots_[curr] = element;
    return TTStatus::Update;
  }

  SizeType next = (lastAccess + 1) % Size;
  if (slots_[next].hash() == element.hash()) {
    slots_[next] = element;
    lastAccess_ = next;
    return TTStatus::Update;
  }

  SizeType next2 = (lastAccess + 2) % Size;
  if (slots_[next2].hash() == element.hash()) {
    slots_[next2] = slots_[next];
    slots_[next] = element;
    lastAccess_ = next;
    return TTStatus::Update;
  }

  SizeType next3 = (lastAccess + 3) % Size;
  if (slots_[next3].hash() == element.hash()) {
    slots_[next3] = slots_[curr];
    slots_[curr] = element;
    lastAccess_ = curr;
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
  slots_[next] = element;
  lastAccess_ = next;
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
