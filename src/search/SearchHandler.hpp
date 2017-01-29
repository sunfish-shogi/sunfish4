/* SearchHandler.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHHANDLER_HPP__
#define SUNFISH_SEARCH_SEARCHHANDLER_HPP__

#include "search/tree/PV.hpp"
#include "search/eval/Score.hpp"

namespace sunfish {

class Searcher;

class SearchHandler {
public:
  virtual void onStart(const Searcher& searcher) = 0;
  virtual void onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) = 0;
  virtual void onFailLow(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) = 0;
  virtual void onFailHigh(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) = 0;
  virtual void onIterateEnd(const Searcher& searcher, float elapsed, int depth) = 0;
};

class LoggingSearchHandler : public SearchHandler {
public:
  void onStart(const Searcher& searcher) override;
  void onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;
  void onFailLow(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;
  void onFailHigh(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;
  void onIterateEnd(const Searcher&, float, int) override {}
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHHANDLER_HPP__
