/* Measure.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_MEASURE_HPP__
#define SUNFISH_SEARCH_TREE_MEASURE_HPP__

#include <cstdint>

#define MEASURE_DEPTH                8
#define MEASURE_NUM_SAMPLE           1000

#define MEASURE_SHOULD(type, depth)  ( \
    g_mes.type.tp[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)] \
  + g_mes.type.fp[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)] \
  + g_mes.type.tn[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)] \
  + g_mes.type.fn[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)] < MEASURE_NUM_SAMPLE)
#define MEASURE_TRUE_POSITIVE(type, depth) (g_mes.type.tp[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)]++)
#define MEASURE_FALSE_POSITIVE(type, depth) (g_mes.type.fp[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)]++)
#define MEASURE_TRUE_NEGATIVE(type, depth) (g_mes.type.tn[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)]++)
#define MEASURE_FALSE_NEGATIVE(type, depth) (g_mes.type.fn[std::max(std::min(depth / Searcher::Depth1Ply, MEASURE_DEPTH-1), 0)]++)

namespace sunfish {

using MeasurementType = uint64_t;

struct Measurement {
  MeasurementType tp[MEASURE_DEPTH]; // true-positive
  MeasurementType fp[MEASURE_DEPTH]; // false-positive
  MeasurementType tn[MEASURE_DEPTH]; // true-negative
  MeasurementType fn[MEASURE_DEPTH]; // false-negative
};

struct Measurements {
  Measurement futilityPruning;
  Measurement LMR;
};

extern Measurements g_mes;

void resetMeasurement();

void continueMeasurement();

void printMeasurementResults();

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_MEASURE_HPP__
