/* TTElement.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TT_TTSLOT_HPP__
#define SUNFISH_TT_TTSLOT_HPP__

#include "search/eval/Score.hpp"
#include "core/move/Move.hpp"
#include "core/position/Zobrist.hpp"
#include <cstdint>
#include <cassert>

#define TT_HASH_WIDTH 16

#define TT_STYPE_MASK ((uint16_t)0x0003)
#define TT_DEPTH_MASK ((uint16_t)0x03fc)
#define TT_MATE_MASK  ((uint16_t)0x0400)

#define TT_STYPE_WIDTH 2
#define TT_DEPTH_WIDTH 8
#define TT_MATE_WIDTH  1

#define TT_STYPE_SHIFT 0
#define TT_DEPTH_SHIFT (TT_STYPE_SHIFT + TT_STYPE_WIDTH)
#define TT_MATE_SHIFT  (TT_DEPTH_SHIFT + TT_DEPTH_WIDTH)

static_assert(TT_STYPE_WIDTH
            + TT_DEPTH_WIDTH
            + TT_MATE_WIDTH <= 16, "invalid data size");
static_assert(TT_MATE_MASK == (((1LLU << TT_MATE_WIDTH) - 1LLU) << TT_MATE_SHIFT), "invalid status");
static_assert(TT_STYPE_MASK == (((1LLU << TT_STYPE_WIDTH) - 1LLU) << TT_STYPE_SHIFT), "invalid status");
static_assert(TT_DEPTH_MASK == (((1LLU << TT_DEPTH_WIDTH) - 1LLU) << TT_DEPTH_SHIFT), "invalid status");

static_assert(sizeof(sunfish::Score::RawType) == 2, "invalid data size");

namespace sunfish {

class TTScoreType {
  TTScoreType() = delete;

public:
  static CONSTEXPR_CONST uint16_t None = 0;
  static CONSTEXPR_CONST uint16_t Upper = 1;
  static CONSTEXPR_CONST uint16_t Lower = 2;
  static CONSTEXPR_CONST uint16_t Exact = Upper | Lower;
};

class TTElement {
private:

  uint16_t hash_;
  uint16_t move_;
  uint16_t score_;
  uint16_t word_;
  uint16_t sum_;

  bool update(Zobrist::Type newHash,
              Score newScore,
              int newScoreType,
              int newDepth,
              int ply,
              Move move,
              bool mateThreat);

  uint16_t calcCheckSum() const {
    return hash_
         ^ move_
         ^ score_
         ^ word_;
  }

public:
  TTElement() :
      hash_(0),
      move_(Move::none().serialize16()),
      score_(0),
      word_(0),
      sum_(0) {
  }

  bool update(Zobrist::Type newHash,
              Score alpha,
              Score beta,
              Score newScore,
              int newDepth, int ply,
              const Move& move,
              bool mateThreat) {
    int newScoreType;
    if (newScore >= beta) {
      newScoreType = TTScoreType::Lower;
    } else if (newScore <= alpha) {
      newScoreType = TTScoreType::Upper;
    } else {
      newScoreType = TTScoreType::Exact;
    }

    return update(newHash,
                  newScore,
                  newScoreType,
                  newDepth,
                  ply,
                  move,
                  mateThreat);
  }

  void updatePV(Zobrist::Type newHash,
                Score newScore,
                int newDepth,
                Move move);

  bool isLive() const {
    return (sum_ ^ calcCheckSum()) == 0LLU;
  }

  bool checkHash(Zobrist::Type hash) const {
    return (static_cast<Zobrist::Type>(hash_) ^ (hash >> (64 - TT_HASH_WIDTH))) == 0LLU && isLive();
  }

  uint16_t hash() const {
    return hash_;
  }

  Score score(int ply) const {
    auto rawValue = static_cast<Score::RawType>(score_);
    Score s(rawValue);

    int st = scoreType();
    ASSERT(st == TTScoreType::None || s >= -Score::infinity());
    ASSERT(st == TTScoreType::None || s <= Score::infinity());

    if (s >= Score::mate()) {
      if (st == TTScoreType::Lower) { return s - ply; }
    } else if (s <= -Score::mate()) {
      if (st == TTScoreType::Upper) { return s + ply; }
    }

    return s;
  }

  uint16_t scoreType() const {
    return (word_ & TT_STYPE_MASK) >> TT_STYPE_SHIFT;
  }

  int depth() const {
    auto data = (word_ & TT_DEPTH_MASK) >> TT_DEPTH_SHIFT;
    return static_cast<int>(data);
  }

  Move move() const {
    auto rawValue = static_cast<Move::RawType16>(move_);
    return Move::deserialize(rawValue);
  }

  bool isMateThreat() const {
    return word_ & TT_MATE_MASK;
  }

};

} // namespace sunfish

#endif // SUNFISH_TT_TTSLOT_HPP__
