/* SearchUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_UTIL_SEARCHUTIL_HPP__
#define SUNFISH_SEARCH_UTIL_SEARCHUTIL_HPP__

#include "search/Searcher.hpp"

namespace sunfish {

class SearchUtil {
public:

  SearchUtil() = delete;

  static void initialize() {
    Searcher::initialize();
  }

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_UTIL_SEARCHUTIL_HPP__
