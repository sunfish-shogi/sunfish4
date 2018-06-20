/* SearchConfig.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHCONFIG_HPP__
#define SUNFISH_SEARCH_SEARCHCONFIG_HPP__

#include "common/Def.hpp"
#include <cstdint>

namespace sunfish {

struct SearchConfig {
  using TimeType = uint32_t;

  static CONSTEXPR_CONST TimeType InfinityTime = ~static_cast<TimeType>(0);
  static CONSTEXPR_CONST TimeType DefaultOptimumTimeMs = 3 * 1000;
  static CONSTEXPR_CONST TimeType DefaultMaximumTimeMs = 3 * 1000;
  static CONSTEXPR_CONST int DefaultNumberOfThreads = 1;
  static CONSTEXPR_CONST int DefaultMultiPV = 1;

  TimeType optimumTimeMs;
  TimeType maximumTimeMs;
  int numberOfThreads;
  int multiPV;
};

inline CONSTEXPR SearchConfig getDefaultSearchConfig() {
  return {
    SearchConfig::DefaultOptimumTimeMs,
    SearchConfig::DefaultMaximumTimeMs,
    SearchConfig::DefaultNumberOfThreads,
    SearchConfig::DefaultMultiPV,
  };
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHCONFIG_HPP__
