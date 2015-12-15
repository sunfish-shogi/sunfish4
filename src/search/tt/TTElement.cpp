/* TTElement.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTElement.hpp"
#include "core/move/Move.hpp"
#include <algorithm>

namespace sunfish {

bool TTElement::update(Zobrist::Type newHash,
                       Score newScore,
                       TTScoreType newScoreType,
                       int newDepth,
                       int ply,
                       Move move,
                       AgeType newAge) {
  assert(newAge < AgeMax);
  assert(newScoreType < (TTScoreType)(1 << TT_VTYPE_WIDTH));

  newDepth = std::max(newDepth, 0);
  newDepth = std::min(newDepth, (1 << TT_DEPTH_WIDTH) - 1);

  // check if a hash value of the current data is equal to a value of new data.
  if (checkHash(newHash)) {
    // reject the data which has shallower depth than the current data.
    if (newDepth < depth() &&
        newAge == age() &&
        newScore < Score::mate() &&
        newScore > -Score::mate()) {
      return false;
    }
    _2 &= TT_MOVE_MASK;
  } else {
    // overwrite
    _2 = Move::empty().serialize16() << TT_MOVE_SHIFT;
  }

  // normalize a score
  if (newScore >= Score::mate()) {
    if (newScoreType == Lower) {
      if (newScore < Score::infinity() - ply) {
        newScore += ply;
      } else {
        newScore = Score::infinity();
      }
    }
  } else if (newScore <= -Score::mate()) {
    if (newScoreType == Upper) {
      if (newScore > -Score::infinity() + ply) {
        newScore -= ply;
      } else {
        newScore = -Score::infinity();
      }
    }
  }

  // 1st quad word
  _1 = newHash & TT_HASH_MASK;
  _1 |= (static_cast<QuadWord>(newAge)) << TT_AGE_SHIFT;

  // 2nd quad word
  _2 |= (static_cast<QuadWord>(newScore.raw())) << TT_VALUE_SHIFT;
  _2 |= (static_cast<QuadWord>(newScoreType)) << TT_VTYPE_SHIFT;
  _2 |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
  if (!move.isEmpty()) {
    _2 &= ~TT_MOVE_MASK;
    _2 |= (static_cast<QuadWord>(move.serialize16())) << TT_MOVE_SHIFT;
  }
  _2 |= calcCheckSum();

  return true;

}

void TTElement::updatePV(Zobrist::Type newHash,
                         int newDepth,
                         Move move,
                         uint32_t newAge) {
  newDepth = std::max(newDepth, 0);

  // check if a hash value of the current data is equal to a value of new data.
  if (checkHash(newHash)) {
    if (newDepth >= depth() || newAge != age()) {
      _2 &= ~(TT_VTYPE_MASK | TT_DEPTH_MASK | TT_CSUM_MASK);
      _2 |= (static_cast<QuadWord>(None)) << TT_VTYPE_SHIFT;
      _2 |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
    } else {
      _2 &= ~(TT_CSUM_MASK);
    }
  } else {
    _2 = Move::empty().serialize16() << TT_MOVE_SHIFT;
    _2 |= (static_cast<QuadWord>(None)) << TT_VTYPE_SHIFT;
    _2 |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
  }

  // 1st quad word
  _1 = newHash & TT_HASH_MASK;
  _1 |= (static_cast<QuadWord>(newAge)) << TT_AGE_SHIFT;

  // 2nd quad word
  if (!move.isEmpty()) {
    _2 &= ~TT_MOVE_MASK;
    _2 |= (static_cast<QuadWord>(move.serialize16())) << TT_MOVE_SHIFT;
  }
  _2 |= calcCheckSum();
}

} // namespace sunfish
