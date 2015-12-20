/* Evaluator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
#define SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__

#include "common/Def.hpp"
#include "core/base/Piece.hpp"
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include "search/eval/Score.hpp"

namespace sunfish {

class Position;

class Evaluator {
public:

  Evaluator();

  Score evaluateMaterial(const Position& position) const;

  Score evaluateDiff(const Position& position, Move move, Piece captured) const {
    if (position.getTurn() == Turn::White) {
      return evaluateDiff<Turn::Black>(position, move, captured);
    } else {
      return evaluateDiff<Turn::White>(position, move, captured);
    }
  }

private:

  template <Turn turn>
  Score evaluateDiff(const Position& position, Move move, Piece captured) const;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
