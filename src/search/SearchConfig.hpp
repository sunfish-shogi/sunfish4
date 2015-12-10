/* SearchConfig.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHCONFIG_HPP__
#define SUNFISH_SEARCH_SEARCHCONFIG_HPP__

#include <cstdint>

namespace sunfish {

struct SearchConfig {
  static CONSTEXPR_CONST uint32_t DefaultOptimumTimeSeconds = 3;
  static CONSTEXPR_CONST uint32_t DefaultMaximumTimeSeconds = 3;

  uint32_t optimumTimeSeconds;
  uint32_t maximumTimeSeconds;
};

inline CONSTEXPR SearchConfig getDefaultSearchConfig() {
  return {
    SearchConfig::DefaultOptimumTimeSeconds,
    SearchConfig::DefaultMaximumTimeSeconds
  };
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHCONFIG_HPP__
