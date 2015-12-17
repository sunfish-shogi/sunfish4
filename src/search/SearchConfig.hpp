/* SearchConfig.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHCONFIG_HPP__
#define SUNFISH_SEARCH_SEARCHCONFIG_HPP__

#include <cstdint>

namespace sunfish {

struct SearchConfig {
  using TimeType = uint32_t;

  static CONSTEXPR_CONST TimeType InfinityTime = ~static_cast<TimeType>(0);
  static CONSTEXPR_CONST TimeType DefaultOptimumMilliSeconds = 3 * 1000;
  static CONSTEXPR_CONST TimeType DefaultMaximumMilliSeconds = 3 * 1000;

  TimeType optimumMilliSeconds;
  TimeType maximumMilliSeconds;
};

inline CONSTEXPR SearchConfig getDefaultSearchConfig() {
  return {
    SearchConfig::DefaultOptimumMilliSeconds,
    SearchConfig::DefaultMaximumMilliSeconds
  };
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHCONFIG_HPP__
