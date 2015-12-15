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

  static CONSTEXPR_CONST unsigned Width = 18;

  static_assert((Width + TT_HASH_WIDTH >= 64), "invalid hash table size");

  TT() : HashTable<TTSlots>(Width), age_(1) {}
  TT(const TT&) = delete;
  TT(TT&&) = delete;

  void evolve() {
    age_ = age_ % (TTElement::AgeMax-1) + 1;
    assert(age_ != TTElement::InvalidAge);
  }

  TTStatus store(Zobrist::Type hash,
                 Score alpha,
                 Score beta,
                 Score score,
                 int depth,
                 int ply,
                 const Move& move) {
    TTElement element;
    TTSlots& slots = getElement(hash);
    slots.get(hash, element);
    if (element.update(hash, alpha, beta, score, depth, ply, move, age_)) {
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
    element.updatePV(hash, depth, move, age_);
    return slots.set(element);
  }

  bool get(Zobrist::Type hash, TTElement& e) {
    return getElement(hash).get(hash, e) && e.checkHash(hash);
  }

private:

  TTElement::AgeType age_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TT_HPP__
