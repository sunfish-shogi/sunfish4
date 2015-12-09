/* SearchInfo.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHINFO_HPP__
#define SUNFISH_SEARCH_SEARCHINFO_HPP__

#include <cstdint>

namespace sunfish {

struct SearchInfo {
  uint64_t nodes;
};

void initializeSearchInfo(SearchInfo& info);

void mergeSearchInfo(SearchInfo& dst, const SearchInfo& src);

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHINFO_HPP__
