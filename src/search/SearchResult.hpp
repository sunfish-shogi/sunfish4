/* SearchResult.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHRESULT_HPP__
#define SUNFISH_SEARCH_SEARCHRESULT_HPP__

#include "core/move/Move.hpp"
#include "search/eval/Value.hpp"
#include "search/tree/PV.hpp"

namespace sunfish {

struct SearchResult {
  Move move;
  Value value;
  PV pv;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHRESULT_HPP__
