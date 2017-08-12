/* TT.h
 * 
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TT_HPP__
#define SUNFISH_SEARCH_TT_HPP__

#include "search/tt/TTSlots.hpp"
#include "search/table/HashTable.hpp"
#include "core/position/Zobrist.hpp"
#include <algorithm>
#include <cstdint>

namespace sunfish {

class TT : public HashTable<TTSlots> {
public:

  static CONSTEXPR_CONST unsigned DefaultWidth = 18;

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

  bool get(Zobrist::Type hash, TTElement& e) {
    return getElement(hash).get(hash, e) &&
           e.checkHash(hash);
  }

  float usageRates() const {
    uint64_t usage = 0;
    auto size = std::min(getSize(), (uint32_t)10000);
    for (SizeType i = 0; i < size; i++) {
      usage += getElement(i).fullCount();
    }
    return static_cast<float>(usage) / (TTSlots::Size * size);
  }

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TT_HPP__
