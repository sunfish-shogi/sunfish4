/* SEE.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEE_SEE_HPP__
#define SUNFISH_SEARCH_SEE_SEE_HPP__

#include "search/tree/Tree.hpp"
#include "search/eval/Score.hpp"
#include "core/move/Move.hpp"

namespace sunfish {

class SEE {
public:

  static Score calculate(const Tree& tree, Move move);

  static void sortMoves(Tree& tree);

private:

  SEE();

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEE_SEE_HPP__
