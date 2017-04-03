/* TTElement.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tt/TTElement.hpp"
#include "core/move/Move.hpp"
#include <algorithm>

namespace sunfish {

bool TTElement::update(Zobrist::Type newHash,
                       Score alpha,
                       Score beta,
                       Score newScore,
                       int newDepth,
                       int ply,
                       Move move,
                       bool mateThreat) {
  int newScoreType;
  if (newScore >= beta) {
    newScoreType = TTScoreType::Lower;
  } else if (newScore <= alpha) {
    newScoreType = TTScoreType::Upper;
  } else {
    newScoreType = TTScoreType::Exact;
  }

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
    word_ &= TT_MATE_MASK;
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

} // namespace sunfish
