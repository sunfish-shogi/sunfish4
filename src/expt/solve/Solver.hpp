/* Solver.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_EXPT_SOLVE_SOLVER_HPP__
#define SUNFISH_EXPT_SOLVE_SOLVER_HPP__

#include "core/position/Position.hpp"
#include "core/move/Move.hpp"
#include "search/Searcher.hpp"
#include <string>
#include <cstdint>

namespace sunfish {

class Solver : LoggingSearchHandler {
public:

  struct Config {
    int muximumDepth;
    SearchConfig::TimeType muximumTimeSeconds;
    int numberOfThreads;
    bool noInterrupt;
  };

  struct Nodes {
    uint64_t nodes;
    uint64_t sample;
  };

  static CONSTEXPR_CONST int MaxDepthOfNodeCount = 10;

  struct Result {
    unsigned corrected;
    unsigned incorrected;
    unsigned mate;
    uint64_t nodes;
    double elapsed;
    Nodes nodesEachDepth[MaxDepthOfNodeCount];
  };

  Solver();

  bool solve(const char* path);

  bool solve(const std::string& path) {
    return solve(path.c_str());
  }

  const Config& getConfig() const {
    return config_;
  }

  void setConfig(const Config& config) {
    config_ = config;
  }

  void onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;

  void onIterateEnd(const Searcher& searcher, float elapsed, int depth) override;

private:

  bool solveCsaFile(const char* path);

  bool solve(const Position& position, Move correct);

private:

  Searcher searcher_;
  Config config_;
  Result result_;
  Move correct_;

};

} // namespace sunfish

#endif // SUNFISH_EXPT_SOLVE_SOLVER_HPP__
