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
  using OFVType = OptimizedFeatureVector<int16_t>;

  enum class InitType {
    EvalBin,
    Zero,
  };

  enum class DataSourceType {
    EvalBin,
    Zero,
    Custom,
  };

  static std::shared_ptr<Evaluator> sharedEvaluator();

  Evaluator(InitType type);

  void initializeZero();

  void onChanged(DataSourceType dataSourceType);

  Score calculateMaterialScore(const Position& position) const;

  Score calculateMaterialScoreDiff(Score score,
                                   const Position& position,
                                   Move move,
                                   Piece captured) const;

  Score calculatePositionalScore(const Position& position);

  Score calculateTotalScore(Score materialScore,
                            const Position& position);

  Score estimateScore(Score score,
                      const Position& position,
                      Move move);

#if !MATERIAL_LEARNING_ONLY
  OFVType& ofv() {
    return ofv_;
  }
#endif // !MATERIAL_LEARNING_ONLY

  DataSourceType dataSourceType() const {
    return dataSourceType_;
  }

private:

  EvalCache cache_;

#if !MATERIAL_LEARNING_ONLY
  OFVType ofv_;
#endif // !MATERIAL_LEARNING_ONLY

  DataSourceType dataSourceType_;

};

bool load(const char* path, Evaluator::FVType& fv);

bool load(Evaluator::FVType& fv);

bool load(const char* path, Evaluator::OFVType& ofv);

bool load(Evaluator::OFVType& ofv);

bool load(const char* path, Evaluator& eval);

bool load(Evaluator& eval);

bool save(const char* path, const Evaluator::FVType& fv);

bool save(const Evaluator::FVType& fv);

bool save(const char* path, const Evaluator::OFVType& ofv);

bool save(const Evaluator::OFVType& ofv);

inline std::ostream& operator<<(std::ostream& os, const Evaluator::DataSourceType dataSourceType) {
  switch (dataSourceType) {
  case Evaluator::DataSourceType::EvalBin:
    os << "EvalBin";
    break;
  case Evaluator::DataSourceType::Zero:
    os << "Zero";
    break;
  case Evaluator::DataSourceType::Custom:
    os << "Custom";
    break;
  default:
    os << static_cast<int>(dataSourceType);
    break;
  }
  return os;
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
