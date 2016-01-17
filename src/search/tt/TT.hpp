/* TT.h
 * 
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TT_HPP__
#define SUNFISH_SEARCH_TT_HPP__

#include "search/tt/TTSlots.hpp"
#include "search/table/HashTable.hpp"
#include "core/position/Zobrist.hpp"
#include <cstdint>

namespace sunfish {

class TT : public HashTable<TTSlots> {
public:

  static CONSTEXPR_CONST unsigned DefaultWidth = 18;

  static_assert((DefaultWidth + TT_HASH_WIDTH >= 64), "invalid hash table size");

  TT() : HashTable<TTSlots>(DefaultWidth) {}
  TT(const TT&) = delete;
  TT(TT&&) = delete;

  TTStatus store(Zobrist::Type hash,
                 Score alpha,
                 Score beta,
                 Score score,
                 int depth,
                 int ply,
                 const Move& move,
                 bool mateThreat) {
    TTElement element;
    TTSlots& slots = getElement(hash);
    slots.get(hash, element);
    if (element.update(hash,
                       alpha,
                       beta,
                       score,
                       depth,
                       ply,
                       move,
                       mateThreat)) {
      return slots.set(element);
    }
    return TTStatus::Reject;
  }

  TTStatus storePV(Zobrist::Type hash,
                   Score score,
                   int depth,
                   const Move& move) {
    TTElement element;
    TTSlots& slots = getElement(hash);
    slots.get(hash, element);
    element.updatePV(hash,
                     score,
                     depth,
                     move);
    return slots.set(element);
  }

  bool get(Zobrist::Type hash, TTElement& e) {
    return getElement(hash).get(hash, e) &&
           e.checkHash(hash);
  }

  float usageRates() const {
    uint64_t usage = 0.0;
    for (SizeType i = 0; i < getSize(); i++) {
      usage += getElement(i).fullCount();
    }
    return static_cast<float>(usage) / TTSlots::Size / getSize();
  }

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TT_HPP__
