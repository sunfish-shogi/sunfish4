/* SearchInfo.cpp
 *
 * Kubo Ryosuke
 */

#include "search/SearchInfo.hpp"
#include <cstring>

namespace sunfish {

void initializeSearchInfo(SearchInfo& info) {
  memset(&info, 0, sizeof(info));
}

void mergeSearchInfo(SearchInfo& dst, const SearchInfo& src) {
  dst.nodes += src.nodes;
  dst.hashCut += src.hashCut;
  dst.nullMovePruning += src.nullMovePruning;
}

} // namespace sunfish
