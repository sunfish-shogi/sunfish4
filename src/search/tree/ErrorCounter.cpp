/* ErrorCounter.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/ErrorCounter.hpp"
#include "common/string/TablePrinter.hpp"
#include "logger/Logger.hpp"
#include <sstream>
#include <cstring>

namespace sunfish {

#if ENABLE_ERR_COUNT
ErrorCounters ecs;
#endif

void printErrorCount() {
  OUT(info) << "ErrorCount:";
  auto print = [](const char* name, ErrorCounter& ec) {
    std::ostringstream oss;
    for (int i = 0; i < 64; i++) {
      auto s = ec.succ[i];
      auto e = ec.err[i];
      if (e >= 3) {
        auto p = s * 100.0f / (s + e);
        oss << i << ')' << std::fixed << std::setprecision(2) << p << "% ";
      }
    }
    OUT(info) << "  " << name << ':';
    OUT(info) << "    " << oss.str();
  };

  print("futilityPruning", ecs.futilityPruning);
  print("LMR", ecs.LMR);
}

} // namespace sunfish
