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

namespace sunfish {

class Solver {
public:

  struct Result {
    unsigned corrected;
    unsigned incorrected;
    unsigned skipped;
  };

  Solver();

  bool solve(const char* path);

  bool solve(const std::string& path) {
    return solve(path.c_str());
  }

private:

  bool solveCsaFile(const char* path);

  bool solve(const Position& position, Move correct);

private:

  LoggingSearchHandler searchHandler_;
  Searcher searcher_;
  Result result_;

};

} // namespace sunfish

#endif // SUNFISH_EXPT_SOLVE_SOLVER_HPP__
