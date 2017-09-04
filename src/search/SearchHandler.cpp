/* SearchHandler.cpp
 *
 * Kubo Ryosuke
 */

#include "search/SearchHandler.hpp"
#include "search/Searcher.hpp"
#include "logger/Logger.hpp"

#include <iomanip>

namespace sunfish {

void LoggingSearchHandler::onStart(const Searcher&) {
}

void LoggingSearchHandler::onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) {
  auto& info = searcher.getInfo();

  auto timeMs = static_cast<uint32_t>(elapsed * 1e3);
  auto realDepth = depth / Searcher::Depth1Ply;

  MSG(info) << std::setw(2) << realDepth << ": "
            << std::setw(10) << (info.nodes + info.quiesNodes) << ": "
            << std::setw(7) << timeMs << ' '
            << pv.toString() << ": "
            << score;
}

void LoggingSearchHandler::onFailLow(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score);
  MSG(info) << "fail-low";
}

void LoggingSearchHandler::onFailHigh(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score);
  MSG(info) << "fail-high";
}
 
} // namespace sunfish
