/* ShekTable.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_SHEK_SHEKTABLE_HPP__
#define SUNFISH_SEACH_SHEK_SHEKTABLE_HPP__

#include "search/shek/ShekSlots.hpp"
#include "search/table/HashTable.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

class ShekTable : public HashTable<ShekSlots> {
public:

  static CONSTEXPR_CONST unsigned Width = 16;

  static_assert(1LLU << Width > ~ShekElement::HashMask, "invalid hash table size");

  ShekTable() : HashTable<ShekSlots>(Width) {}
  ShekTable(const ShekTable&) = delete;
  ShekTable(ShekTable&&) = delete;

  ShekState check(const Position& position) {
    auto hash = position.getBoardHash();
    auto& slots = getElement(hash);
    auto element = slots.findSlot(hash);
    if (element == nullptr) {
      return ShekState::None;
    }

    HandSet handSet(position.getBlackHand());
    return element->check(handSet,
                          position.getTurn());
  }

  void retain(const Position& position, bool inSearchNode) {
    auto hash = position.getBoardHash();
    auto& slots = getElement(hash);
    HandSet handSet(position.getBlackHand());

    auto element = slots.findSlot(hash, handSet);
    if (element != nullptr) {
      element->retain(inSearchNode);
      return;
    }

    element = slots.findVacantSlot();
    if (element != nullptr) {
      element->setAndRetain(hash,
                            handSet,
                            position.getTurn(),
                            inSearchNode);
    }
  }

  void release(const Position& position) {
    auto hash = position.getBoardHash();
    auto& slots = getElement(hash);
    HandSet handSet(position.getBlackHand());

    auto element = slots.findSlot(hash, handSet);
    if (element != nullptr) {
      element->release();
    }
  }

};

} // namespace sunfish

#endif // SUNFISH_SEACH_SHEK_SHEKTABLE_HPP__
