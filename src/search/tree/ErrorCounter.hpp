/* ErrorCounter.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_ERROR_DETECTOR_HPP__
#define SUNFISH_SEARCH_ERROR_DETECTOR_HPP__

#include <cstdint>

#define EC_DEPTH                64
#define EC_NUM_SAMPLE           10

#define EC_SHOULD(type, depth)  ( \
    g_ecs.type.succ[std::max(std::min(depth, EC_DEPTH-1), 0)] \
  + g_ecs.type.err [std::max(std::min(depth, EC_DEPTH-1), 0)] < EC_NUM_SAMPLE)
#define EC_SUCCESS(type, depth) (g_ecs.type.succ[std::max(std::min(depth, EC_DEPTH-1), 0)]++)
#define EC_ERROR(type, depth)   (g_ecs.type.err [std::max(std::min(depth, EC_DEPTH-1), 0)]++)

namespace sunfish {

using ErrorCounterType = uint64_t;

struct ErrorCounter {
  ErrorCounterType succ[EC_DEPTH];
  ErrorCounterType err[EC_DEPTH];
};

struct ErrorCounters {
  ErrorCounter futilityPruning;
  ErrorCounter LMR;
};

extern ErrorCounters g_ecs;

void resetErrorCounts();

void continueErrorCounts();

void printErrorCounts();

} // namespace sunfish

#endif // SUNFISH_SEARCH_ERROR_DETECTOR_HPP__
