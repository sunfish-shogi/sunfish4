/* Evaluator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
#define SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__

#include "common/Def.hpp"
#include "search/eval/Value.hpp"

namespace sunfish {

class Position;

class Evaluator {
public:

  Evaluator();

  Value evaluateMaterial(const Position& position) const;

private:

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
