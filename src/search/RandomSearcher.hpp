/* RandomSearcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_RANDOMSEARCHER_HPP__
#define SUNFISH_SEARCH_RANDOMSEARCHER_HPP__

#include "common/math/Random.hpp"

namespace sunfish {

class Position;
class Move;

class RandomSearcher {
public:

  RandomSearcher();

  bool search(const Position& pos, Move& move);

private:

  Random random_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_RANDOMSEARCHER_HPP__
