/* TTSlots.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTSlots.hpp"
#include <climits>

namespace sunfish {

TTStatus TTSlots::set(const TTElement& element) {
  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    if (slots_[i].hash() == element.hash()) {
      slots_[i] = element;
      return TTStatus::Update;
    }
  }

  // find lesser slot
  TTElement dummy;
  TTElement* e = &dummy;
  int minDepth = INT_MAX;
  for (SizeType i = 0; i < Size; i++) {
    int depth = slots_[i].depth();
    if (depth < minDepth) {
      e = &slots_[i];
      minDepth = depth;
    }
  }
  *e = element;

  return TTStatus::Replace;
}

bool TTSlots::get(Zobrist::Type hash, TTElement& element) {
  // search a slot which has a same hash value.
  for (SizeType i = 0; i < Size; i++) {
    TTElement e = slots_[i];
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
