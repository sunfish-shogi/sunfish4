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
#include "search/eval/Score.hpp"
#include <memory>
#include <cstdint>

namespace sunfish {

class Position;

inline Score::RawType positionalScoreScale() {
  return 32;
}

struct ClassifiedScores {
  Score materialScore;
  Score positionalScore;

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

inline Score calculateScore(const ClassifiedScores& scores) {
  return scores.materialScore + scores.positionalScore / positionalScoreScale();
}

namespace KKP_ {
enum Type {
  HPawn   = 0,
  HLance  = HPawn   + 18,
  HKnight = HLance  + 4,
  HSilver = HKnight + 4,
  HGold   = HSilver + 4,
  HBishop = HGold   + 4,
  HRook   = HBishop + 2,
  BPawn   = HRook   + 2,
  BLance  = BPawn   + 81 - 9,
  BKnight = BLance  + 81 - 9,
  BSilver = BKnight + 81 - 18,
  BGold   = BSilver + 81,
  BBishop = BGold   + 81,
  BHorse  = BBishop + 81,
  BRook   = BHorse  + 81,
  BDragon = BRook   + 81,

  HandEnd = HRook   + 2,
  End     = BDragon + 81,
  Size    = 81 * 81 * End,
};
} // namespace KKP_
using KKP = KKP_::Type;

int symmetricalKkpIndex(int index);

class Evaluator {
public:

  using Type = int16_t;
  using Kkp = Type[81][81][KKP::End];
  using KkpC = Type[81][81][KKP::HandEnd];

  static std::shared_ptr<Evaluator> sharedEvaluator();

  Evaluator();

  void initializeZero();

  template <class T>
  void initializeRandom(T&& gen) {
    for (int i = 0; i < KKP::End; i++) {
      (&kkp_[0][0][0])[i] = gen();
    }
    onChanged();
  }

  bool read(const char* path);

  bool write(const char* path) const;

  void onChanged();

  ClassifiedScores evaluate(const Position& position) const;

  ClassifiedScores evaluateDiff(const ClassifiedScores& scores,
                                const Position& position,
                                Move move,
                                Piece captured) const {
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

  template <Turn turn>
  ClassifiedScores evaluateDiff(ClassifiedScores scores,
                                const Position& position,
                                Move move,
                                Piece captured) const;

private:

  Kkp kkp_;
  KkpC kkpc_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALUATOR_HPP__
