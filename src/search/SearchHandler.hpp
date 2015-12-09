/* SearchHandler.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHHANDLER_HPP__
#define SUNFISH_SEARCH_SEARCHHANDLER_HPP__

#include "search/tree/PV.hpp"
#include "search/eval/Score.hpp"

namespace sunfish {

class SearchHandler {
public:
  virtual void onStart() = 0;
  virtual void onUpdatePV(const PV& pv, int depth, Score score) = 0;
  virtual void onFailLow(const PV& pv, int depth, Score score) = 0;
  virtual void onFailHigh(const PV& pv, int depth, Score score) = 0;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHHANDLER_HPP__
