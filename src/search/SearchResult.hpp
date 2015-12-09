/* SearchResult.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHRESULT_HPP__
#define SUNFISH_SEARCH_SEARCHRESULT_HPP__

#include "core/move/Move.hpp"
#include "search/eval/Score.hpp"
#include "search/tree/PV.hpp"

namespace sunfish {

struct SearchResult {
  Move move;
  Score score;
  PV pv;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHRESULT_HPP__
