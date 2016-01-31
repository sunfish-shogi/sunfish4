/* ErrorCounter.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_ERROR_DETECTOR_HPP__
#define SUNFISH_SEARCH_ERROR_DETECTOR_HPP__

#include <cstdint>

namespace sunfish {

struct ErrorCounter {
  uint64_t succ[64];
  uint64_t err[64];
};

struct ErrorCounters {
  ErrorCounter futilityPruning;
  ErrorCounter LMR;
};

extern ErrorCounters ecs;

void printErrorCount();

#define EC_SUCCESS(type, depth) (ecs.type.succ[std::max(std::min(depth, 63), 0)]++)

#define EC_ERROR(type, depth) (ecs.type.err[std::max(std::min(depth, 63), 0)]++)

} // namespace sunfish

#endif // SUNFISH_SEARCH_ERROR_DETECTOR_HPP__
