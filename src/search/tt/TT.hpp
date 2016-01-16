/* TT.h
 * 
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TT_HPP__
#define SUNFISH_SEARCH_TT_HPP__

#include "search/tt/TTSlots.hpp"
#include "search/table/HashTable.hpp"
#include "core/position/Zobrist.hpp"

namespace sunfish {

class TT : public HashTable<TTSlots> {
public:

  static CONSTEXPR_CONST unsigned Width = 19;

  static_assert((Width + TT_HASH_WIDTH >= 64), "invalid hash table size");

  TT() : HashTable<TTSlots>(Width) {}
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
    if (element.update(hash, alpha, beta, score, depth, ply, move, mateThreat)) {
      return slots.set(element);
    }
    return TTStatus::Reject;
  }

  TTStatus storePV(Zobrist::Type hash,
                   int depth,
                   const Move& move) {
    TTElement element;
    TTSlots& slots = getElement(hash);
    slots.get(hash, element);
    element.updatePV(hash, depth, move);
    return slots.set(element);
  }

  bool get(Zobrist::Type hash, TTElement& e) {
    return getElement(hash).get(hash, e) &&
           e.checkHash(hash);
  }

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TT_HPP__
