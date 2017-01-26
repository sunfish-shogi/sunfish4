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
                       int newScoreType,
                       int newDepth,
                       int ply,
                       Move move,
                       bool mateThreat) {
  ASSERT(newScoreType < (1 << TT_STYPE_WIDTH));

  newDepth = std::max(newDepth, 0);
  newDepth = std::min(newDepth, (1 << TT_DEPTH_WIDTH) - 1);

  // check if the hash value of the current data is equal to
  if (checkHash(newHash)) {
    // reject the data which has shallower depth than the current data.
    if (newDepth < depth() &&
        newScore < Score::mate() &&
        newScore > -Score::mate()) {
      return false;
    }
    word_ &= TT_MATE_WIDTH;
  } else {
    // overwrite
    move_ = Move::none().serialize16();
    word_ = 0x00;
  }

  // normalize the score
  if (newScore >= Score::mate()) {
    newScore = newScore < Score::infinity() - ply ? newScore + ply
                                                  : Score::infinity();
  } else if (newScore <= -Score::mate()) {
    newScore = newScore > -Score::infinity() + ply ? newScore - ply
                                                   : -Score::infinity();
  }

  hash_ = newHash >> (64 - TT_HASH_WIDTH);
  if (!move.isNone()) {
    move_ = move.serialize16();
  }
  score_ = newScore.raw();
  word_ |= static_cast<uint16_t>(mateThreat) << TT_MATE_SHIFT;
  word_ |= static_cast<uint16_t>(newScoreType) << TT_STYPE_SHIFT;
  word_ |= static_cast<uint16_t>(newDepth) << TT_DEPTH_SHIFT;
  sum_ = calcCheckSum();

  return true;

}

void TTElement::updatePV(Zobrist::Type newHash,
                         Score newScore,
                         int newDepth,
                         Move move) {
  newDepth = std::max(newDepth, 0);

  // check if a hash value of the current data is equal to a value of new data.
  if (checkHash(newHash)) {
    if (newDepth < depth()) {
      return;
    }
  }

  hash_ = newHash >> (64 - TT_HASH_WIDTH);
  move_ = move.serialize16();
  score_ = newScore.raw();
  word_  = static_cast<uint16_t>(TTScoreType::Exact) << TT_STYPE_SHIFT;
  word_ |= static_cast<uint16_t>(newDepth) << TT_DEPTH_SHIFT;
  sum_ = calcCheckSum();
}

} // namespace sunfish
