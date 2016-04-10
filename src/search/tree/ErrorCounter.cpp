/* ErrorCounter.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/ErrorCounter.hpp"
#include "search/Searcher.hpp"
#include "common/string/TablePrinter.hpp"
#include "logger/Logger.hpp"
#include <sstream>
#include <list>
#include <cstring>

namespace sunfish {

#if ENABLE_ERR_RATE
ErrorCounters g_ecs;
std::list<ErrorCounters> g_ecsList;
#endif

void resetErrorCounts() {
#if ENABLE_ERR_RATE
  memset(&g_ecs, 0, sizeof(g_ecs));
  g_ecsList.clear();
#endif
}

void continueErrorCounts() {
#if ENABLE_ERR_RATE
  g_ecsList.push_back(g_ecs);
  memset(&g_ecs, 0, sizeof(g_ecs));
#endif
}

void printErrorRate() {
#if ENABLE_ERR_RATE
  OUT(info) << "error counts:";

  auto print = [](const char* name, const ErrorCounter& ec) {
    std::ostringstream oss;
    float maxDepth = 0;
    for (int d = 0; d < EC_DEPTH; d++) {
      auto s = ec.succ[d];
      auto e = ec.err[d];
      auto depth = (float)d / Searcher::Depth1Ply;

      if (s + e < EC_NUM_SAMPLE) {
        continue;
      }

      maxDepth = depth;

      if (e == 0) {
        continue;
      }

      auto p = e * 100.0f / (s + e);
      oss << std::fixed << std::setprecision(2) << p << "%";
      oss << "(dep=" << std::fixed << std::setprecision(2) << depth << ") ";
    }

    OUT(info) << "  " << name << ':';
    OUT(info) << "    " << oss.str() << " (max_dep=" << maxDepth << ")";
  };

  auto ecs = g_ecs;
  for (const auto& e : g_ecsList) {
    for (size_t i = 0; i < sizeof(ErrorCounters) / sizeof(ErrorCounterType); i++) {
      reinterpret_cast<ErrorCounterType*>(&ecs)[i] += reinterpret_cast<const ErrorCounterType*>(&e)[i];
    }
  }

  print("futility pruning", ecs.futilityPruning);
  print("LMR", ecs.LMR);
#endif
}

} // namespace sunfish
