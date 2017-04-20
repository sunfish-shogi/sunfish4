/* RandomSearcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_RANDOMSEARCHER_HPP__
#define SUNFISH_SEARCH_RANDOMSEARCHER_HPP__

namespace sunfish {

class Position;
class Move;

class RandomSearcher {
public:

  RandomSearcher();

  bool search(const Position& pos, Move& move);

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_RANDOMSEARCHER_HPP__
