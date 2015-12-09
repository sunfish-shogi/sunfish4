/* Evaluator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
#define SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__

#include "common/Def.hpp"
#include "search/eval/Score.hpp"

namespace sunfish {

class Position;

class Evaluator {
public:

  Evaluator();

  Score evaluateMaterial(const Position& position) const;

private:

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
