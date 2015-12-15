/* SEE.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEE_SEE_HPP__
#define SUNFISH_SEARCH_SEE_SEE_HPP__

#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"
#include "core/move/Move.hpp"
#include "search/eval/Score.hpp"

namespace sunfish {

class SEE {
public:

  static Score calculate(const Position& position,
                         Move move);

  static void sortMoves(const Position& position,
                        Moves::iterator begin,
                        Moves::iterator end);

private:

  SEE();

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEE_SEE_HPP__
