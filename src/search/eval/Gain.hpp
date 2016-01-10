/* Gain.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_GAIN_HPP__
#define SUNFISH_SEARCH_EVAL_GAIN_HPP__

#include "core/base/Piece.hpp"
#include "core/base/Square.hpp"
#include "core/move/Move.hpp"
#include "search/eval/Score.hpp"
#include <cstdint>
#include <cstring>

namespace sunfish {

class Gain {
public:

  void clear() {
    memset(&gains_[0][0], 0, sizeof(gains_));
  }

  void update(const Move& move,
              const Piece& piece,
              const Score& gain) {
    assert(piece.raw() < PieceNumber::Num);
    assert(move.to().raw() >= 0);
    assert(move.to().raw() < Square::N);
    Score& ref = gains_[piece.raw()][move.to().raw()];
    ref = std::max(ref - 1, gain);
  }

  Score get(const Move& move,
            const Piece& piece) {
    assert(piece.raw() < PieceNumber::Num);
    assert(move.to().raw() >= 0);
    assert(move.to().raw() < Square::N);
    Score g = gains_[piece.raw()][move.to().raw()];
    return g;
  }

private:

  Score gains_[PieceNumber::Num][Square::N];

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_GAIN_HPP__
