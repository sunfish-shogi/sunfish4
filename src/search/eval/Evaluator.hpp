/* Evaluator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
#define SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__

#include "common/Def.hpp"
#include "core/base/Piece.hpp"
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include "search/eval/FeatureVector.hpp"
#include "search/eval/EvalCache.hpp"
#include "search/eval/Score.hpp"
#include <memory>
#include <cstdint>

namespace sunfish {

inline int32_t positionalScoreScale() {
  return 32;
}

class Evaluator {
public:

  using FVType = FeatureVector<int16_t>;
  using CVType = CumulatedVector<int16_t>;

  enum class InitType {
    EvalBin,
    Zero,
  };

  static std::shared_ptr<Evaluator> sharedEvaluator();

  Evaluator(InitType type);

  void initializeZero();

  bool read(const char* path);

  bool write(const char* path) const;

  bool readEvalBin();

  bool writeEvalBin() const;

  void onChanged();

  Score calculateMaterialScore(const Position& position) const;

  Score calculateMaterialScoreDiff(Score score,
                                   const Position& position,
                                   Move move,
                                   Piece captured) const;

  Score calculateTotalScore(Score materialScore,
                            const Position& position);

  Score estimateScore(Score score,
                      const Position& position,
                      Move move);

  FVType& fv() {
    return fv_;
  }

  CVType& cv() {
    return cv_;
  }

private:

  int32_t calculatePositionalScore(const Position& position);

private:

  EvalCache cache_;

  FVType fv_;
  CVType cv_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
