/* TimeManager.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TIMEMANAGER_HPP__
#define SUNFISH_SEARCH_TIMEMANAGER_HPP__

#include "search/SearchConfig.hpp"
#include "search/eval/Score.hpp"
#include "search/tree/PV.hpp"
#include <memory>
#include <cstdint>

namespace sunfish {

class TimeManager {
private:

  struct History {
    bool enable;
    int depth;
    Score score;
    PV pv;
  };

public:

  TimeManager();

  void clearGame();

  void clearPosition(SearchConfig::TimeType optimumTimeMs,
                     SearchConfig::TimeType maximumTimeMs);

  void update(uint32_t elapsedMs,
              int depth,
              Score score,
              const PV& pv);

  bool shouldInterrupt() {
    return shouldInterrupt_;
  }

private:

  SearchConfig::TimeType optimumTimeMs_;
  SearchConfig::TimeType maximumTimeMs_;
  bool shouldInterrupt_;

  std::unique_ptr<History> previous2_;
  std::unique_ptr<History> previous_;
  std::unique_ptr<History> current_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TIMEMANAGER_HPP__
