/* Measure.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Measure.hpp"
#include "search/Searcher.hpp"
#include "common/string/TablePrinter.hpp"
#include "logger/Logger.hpp"
#include <sstream>
#include <list>
#include <cstring>

namespace sunfish {

#if ENABLE_MEASUREMENT
Measurements g_mes;
std::list<Measurements> g_mesList;
#endif

void resetMeasurement() {
#if ENABLE_MEASUREMENT
  memset(&g_mes, 0, sizeof(g_mes));
  g_mesList.clear();
#endif
}

void continueMeasurement() {
#if ENABLE_MEASUREMENT
  g_mesList.push_back(g_mes);
  memset(&g_mes, 0, sizeof(g_mes));
#endif
}

void printMeasurementResults() {
#if ENABLE_MEASUREMENT
  OUT(info) << "measurement results:";

  auto print = [](const char* name, const Measurement& mes) {
    OUT(info) << "  " << name << ':';

    for (int d = 0; d < MEASURE_DEPTH; d++) {
      auto tp = mes.tp[d];
      auto fp = mes.fp[d];
      auto tn = mes.tn[d];
      auto fn = mes.fn[d];

      if (tp + fp + tn + fn < MEASURE_NUM_SAMPLE) {
        continue;
      }

      auto p = (float)tp / (tp + fp);
      auto r = (float)tp / (tp + fn);
      auto f = 2.0f * p * r / (p + r);
      OUT(info) << "    " << d << ":"
          << " precision=" << std::fixed << std::setprecision(3) << p
          << " recall=" << std::fixed << std::setprecision(3) << r
          << " F-measure=" << std::fixed << std::setprecision(3) << f;
    }
  };

  auto mes = g_mes;
  for (const auto& e : g_mesList) {
    for (size_t i = 0; i < sizeof(Measurements) / sizeof(MeasurementType); i++) {
      reinterpret_cast<MeasurementType*>(&mes)[i] += reinterpret_cast<const MeasurementType*>(&e)[i];
    }
  }

  print("futility pruning", mes.mFutilityPruning);
  print("LMR", mes.mLMR);
  print("null move pruning", mes.mNullMovePruning);
#endif
}

} // namespace sunfish
