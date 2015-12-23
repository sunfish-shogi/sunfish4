/* SearchInfo.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHINFO_HPP__
#define SUNFISH_SEARCH_SEARCHINFO_HPP__

#include <cstdint>
#include <cstring>

namespace sunfish {

struct SearchInfo {
  uint64_t nodes;
  uint64_t quiesNodes;
  uint64_t hashCut;
  uint64_t nullMovePruning;
};

inline void initializeSearchInfo(SearchInfo& info) {
  memset(&info, 0, sizeof(info));
}

inline void mergeSearchInfo(SearchInfo& dst, const SearchInfo& src) {
  dst.nodes += src.nodes;
  dst.quiesNodes += src.quiesNodes;
  dst.hashCut += src.hashCut;
  dst.nullMovePruning += src.nullMovePruning;
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHINFO_HPP__
