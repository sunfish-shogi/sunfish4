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
#include "search/eval/Score.hpp"
#include <memory>
#include <cstdint>

namespace sunfish {

struct ClassifiedScores {
  Score materialScore;
  int32_t positionalScore;

  ClassifiedScores operator-() {
    return { -materialScore, -positionalScore };
  }
  ClassifiedScores operator+(const ClassifiedScores& rhs) {
    return {
      materialScore   + rhs.materialScore,
      positionalScore + rhs.positionalScore
    };
  }
};

inline int32_t positionalScoreScale() {
  return 32;
}

inline Score calculateScore(const ClassifiedScores& scores) {
  return scores.materialScore + static_cast<Score::RawType>(scores.positionalScore / positionalScoreScale());
}

class Evaluator {
public:

  using FeatureType = FeatureVector<int16_t>;

  static std::shared_ptr<Evaluator> sharedEvaluator();

  Evaluator();

  void initializeZero();

  template <class T>
  void initializeRandom(T&& gen) {
    for (int i = 0; i < KKP::End; i++) {
      (&fv_.kkp[0][0][0])[i] = gen();
    }
    onChanged();
  }

  bool read(const char* path);

  bool write(const char* path) const;

  void onChanged();

  ClassifiedScores evaluate(const Position& position);

  ClassifiedScores evaluateDiff(const ClassifiedScores& scores,
                                const Position& position,
                                Move move,
                                Piece captured) {
    if (position.getTurn() == Turn::White) {
      return evaluateDiff<Turn::Black>(scores,
                                       position,
                                       move,
                                       captured);
    } else {
      return evaluateDiff<Turn::White>(scores,
                                       position,
                                       move,
                                       captured);
    }
  }

private:

  Score calculateMaterialScore(const Position& position);

  int32_t calculatePositionalScore(const Position& position);

  template <Turn turn>
  ClassifiedScores evaluateDiff(ClassifiedScores scores,
                                const Position& position,
                                Move move,
                                Piece captured);

private:

  FeatureType fv_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
