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
  assert(newAge <= MaxAge);
  assert(newScoreType < (TTScoreType)(1 << TT_STYPE_WIDTH));

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
    w2_ &= TT_MOVE_MASK;
  } else {
    // overwrite
    w2_ = Move::empty().serialize16() << TT_MOVE_SHIFT;
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
  w1_ = newHash & TT_HASH_MASK;
  w1_ |= (static_cast<QuadWord>(newAge)) << TT_AGE_SHIFT;

  // 2nd quad word
  auto scoreU16 = static_cast<uint16_t>(newScore.raw());
  w2_ |= (static_cast<QuadWord>(scoreU16)) << TT_SCORE_SHIFT;
  w2_ |= (static_cast<QuadWord>(newScoreType)) << TT_STYPE_SHIFT;
  w2_ |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
  if (!move.isEmpty()) {
    w2_ &= ~TT_MOVE_MASK;
    w2_ |= (static_cast<QuadWord>(move.serialize16())) << TT_MOVE_SHIFT;
  }
  w2_ |= calcCheckSum() & TT_CSUM_MASK;

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
      w2_ &= ~(TT_STYPE_MASK | TT_DEPTH_MASK | TT_CSUM_MASK);
      w2_ |= (static_cast<QuadWord>(None)) << TT_STYPE_SHIFT;
      w2_ |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
    } else {
      w2_ &= ~(TT_CSUM_MASK);
    }
  } else {
    w2_ = Move::empty().serialize16() << TT_MOVE_SHIFT;
    w2_ |= (static_cast<QuadWord>(None)) << TT_STYPE_SHIFT;
    w2_ |= (static_cast<QuadWord>(newDepth)) << TT_DEPTH_SHIFT;
  }

  // 1st quad word
  w1_ = newHash & TT_HASH_MASK;
  w1_ |= (static_cast<QuadWord>(newAge)) << TT_AGE_SHIFT;

  // 2nd quad word
  if (!move.isEmpty()) {
    w2_ &= ~TT_MOVE_MASK;
    w2_ |= (static_cast<QuadWord>(move.serialize16())) << TT_MOVE_SHIFT;
  }
  w2_ |= calcCheckSum() & TT_CSUM_MASK;
}

} // namespace sunfish
