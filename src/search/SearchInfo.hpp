/* SearchInfo.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHINFO_HPP__
#define SUNFISH_SEARCH_SEARCHINFO_HPP__

#include <iomanip>
#include <cstdint>
#include <cstring>

namespace sunfish {

struct SearchInfo {
  uint64_t nodes;
  uint64_t quiesNodes;
  uint64_t hashCut;
  uint64_t nullMovePruning;
  uint64_t futilityPruning;
  uint64_t failHigh;
  uint64_t failHighFirst;
};

inline void initializeSearchInfo(SearchInfo& info) {
  memset(&info, 0, sizeof(info));
}

inline void mergeSearchInfo(SearchInfo& dst, const SearchInfo& src) {
  dst.nodes           += src.nodes;
  dst.quiesNodes      += src.quiesNodes;
  dst.hashCut         += src.hashCut;
  dst.nullMovePruning += src.nullMovePruning;
  dst.futilityPruning += src.futilityPruning;
  dst.failHigh        += src.failHigh;
  dst.failHighFirst   += src.failHighFirst;
}

template <class T>
inline void printSearchInfo(T& os, const SearchInfo& info, float elapsed) {
  auto totalNodes = info.nodes + info.quiesNodes;
  auto nps = static_cast<uint32_t>(totalNodes / elapsed);
  auto failHighFirst = info.failHighFirst * 100 / (info.failHigh + 1);

  os << "nps               : " << nps;
  os << "elapsed           : " << std::fixed << std::setprecision(3) << elapsed;
  os << "nodes             : " << info.nodes;
  os << "quies nodes       : " << info.quiesNodes;
  os << "total nodes       : " << totalNodes;
  os << "hash-cut          : " << info.hashCut;
  os << "null move pruning : " << info.nullMovePruning;
  os << "futility pruning  : " << info.futilityPruning;
  os << "fail high first   : " << failHighFirst << "%";
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHINFO_HPP__
