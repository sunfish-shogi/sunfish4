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

// 1st quad word
#define TT_AGE_MASK    0x0000000000000007LLU
#define TT_MATE_MASK   0x0000000000000008LLU
#define TT_HASH_MASK   0xfffffffffffffff0LLU

#define TT_AGE_WIDTH    3
#define TT_MATE_WIDTH   1
#define TT_HASH_WIDTH  60

#define TT_AGE_SHIFT   0
#define TT_MATE_SHIFT  (TT_AGE_SHIFT + TT_AGE_WIDTH)

static_assert(TT_AGE_WIDTH
            + TT_MATE_WIDTH
            + TT_HASH_WIDTH <= 64, "invalid data size");
static_assert(TT_AGE_MASK  == (((1LLU << TT_AGE_WIDTH) - 1LLU) << TT_AGE_SHIFT), "invalid status");
static_assert(TT_MATE_MASK == (((1LLU << TT_MATE_WIDTH) - 1LLU) << TT_MATE_SHIFT), "invalid status");
static_assert(TT_HASH_MASK == (~((1LLU << (64 - TT_HASH_WIDTH)) - 1)), "invalid status");

// 2nd quad word
#define TT_MOVE_MASK  0x000000000000ffffLLU
#define TT_SCORE_MASK 0x00000000ffff0000LLU
#define TT_STYPE_MASK 0x0000000300000000LLU
#define TT_DEPTH_MASK 0x00000ffc00000000LLU
#define TT_CSUM_MASK  0xffff000000000000LLU

#define TT_MOVE_WIDTH  16
#define TT_SCORE_WIDTH 16
#define TT_STYPE_WIDTH  2
#define TT_DEPTH_WIDTH 10
#define TT_CSUM_WIDTH  16

#define TT_MOVE_SHIFT  0
#define TT_SCORE_SHIFT (TT_MOVE_SHIFT + TT_MOVE_WIDTH)
#define TT_STYPE_SHIFT (TT_SCORE_SHIFT + TT_SCORE_WIDTH)
#define TT_DEPTH_SHIFT (TT_STYPE_SHIFT + TT_STYPE_WIDTH)

static_assert(TT_MOVE_WIDTH
            + TT_SCORE_WIDTH
            + TT_STYPE_WIDTH
            + TT_DEPTH_WIDTH
            + TT_CSUM_WIDTH <= 64, "invalid data size");
static_assert(TT_MOVE_MASK  == (((1LLU << TT_MOVE_WIDTH) - 1) << TT_MOVE_SHIFT), "invalid status");
static_assert(TT_SCORE_MASK == (((1LLU << TT_SCORE_WIDTH) - 1) << TT_SCORE_SHIFT), "invalid status");
static_assert(TT_STYPE_MASK == (((1LLU << TT_STYPE_WIDTH) - 1) << TT_STYPE_SHIFT), "invalid status");
static_assert(TT_DEPTH_MASK == (((1LLU << TT_DEPTH_WIDTH) - 1) << TT_DEPTH_SHIFT), "invalid status");
static_assert(TT_CSUM_MASK  == (~((1LLU << (64 - TT_CSUM_WIDTH)) - 1)), "invalid status");

static_assert(sizeof(sunfish::Score::RawType) == 2, "invalid data size");

static_assert(TT_CSUM_WIDTH == 16, "invalid data size");
static_assert(TT_CSUM_MASK == 0xffff000000000000LLU, "invalid data size");

namespace sunfish {

enum class TTStatus : int {
  None,
  Reject,
  New,
  Update,
  Collide,
};

enum class TTScoreType : int {
  Exact = 0,
  Upper, /* = 1 */
  Lower, /* = 2 */
  None, /* = 3 */
};

class TTElement {
public:

  using AgeType = uint32_t;
  using QuadWord = uint64_t;

  static CONSTEXPR_CONST AgeType MaxAge = (0x01 << TT_AGE_WIDTH) - 1;

private:

  QuadWord w1_;
  QuadWord w2_;

  bool update(Zobrist::Type newHash,
              Score newScore,
              TTScoreType newScoreType,
              int newDepth,
              int ply,
              Move move,
              bool mateThreat,
              AgeType newAge);

  QuadWord calcCheckSum() const {
    return
      (
        (w1_) ^
        (w1_ << 16) ^
        (w1_ << 32) ^
        (w1_ << 48) ^
        (w2_ << 16) ^
        (w2_ << 32) ^
        (w2_ << 48)
      );
  }

public:
  TTElement() :
      w1_(0),
      w2_(~(calcCheckSum() & TT_CSUM_MASK)) {
  }

  bool update(Zobrist::Type newHash,
      Score alpha,
      Score beta,
      Score newScore,
      int newDepth, int ply,
      const Move& move,
      bool mateThreat,
      AgeType newAge) {
    TTScoreType newScoreType;
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
                  mateThreat,
                  newAge);
  }

  void updatePV(Zobrist::Type newHash,
                int newDepth,
                Move move,
                AgeType newAge);

  bool checkHash(Zobrist::Type hash) const {
    return ((w1_ ^ hash          ) & TT_HASH_MASK) == 0LLU
        && ((w2_ ^ calcCheckSum()) & TT_CSUM_MASK) == 0LLU;
  }

  Zobrist::Type hash() const {
    return w1_ & TT_HASH_MASK;
  }

  Score score(int ply) const {
    auto data = (w2_ & TT_SCORE_MASK) >> TT_SCORE_SHIFT;
    auto u16 = static_cast<uint16_t>(data);
    auto rawValue = static_cast<Score::RawType>(u16);
    Score s(rawValue);

    TTScoreType st = scoreType();
    ASSERT(st == TTScoreType::None || s >= -Score::infinity());
    ASSERT(st == TTScoreType::None || s <= Score::infinity());

    if (s >= Score::mate()) {
      if (st == TTScoreType::Lower) { return s - ply; }
    } else if (s <= -Score::mate()) {
      if (st == TTScoreType::Upper) { return s + ply; }
    }

    return s;
  }

  TTScoreType scoreType() const {
    auto data = (w2_ & TT_STYPE_MASK) >> TT_STYPE_SHIFT;
    return static_cast<TTScoreType>(data);
  }

  int depth() const {
    auto data = (w2_ & TT_DEPTH_MASK) >> TT_DEPTH_SHIFT;
    return static_cast<int>(data);
  }

  Move move() const {
    auto data = (w2_ & TT_MOVE_MASK) >> TT_MOVE_SHIFT;
    auto rawValue = static_cast<Move::RawType16>(data);
    return Move::deserialize(rawValue);
  }

  AgeType age() const {
    auto data = (w1_ & TT_AGE_MASK) >> TT_AGE_SHIFT;
    return static_cast<AgeType>(data);
  }

  bool isMateThreat() const {
    return w1_ & TT_MATE_MASK;
  }

};

} // namespace sunfish

inline std::ostream& operator<<(std::ostream& os, sunfish::TTScoreType scrState) {
  switch (scrState) {
  case sunfish::TTScoreType::Exact:
    os << "Exact";
    break;
  case sunfish::TTScoreType::Upper:
    os << "Upper";
    break;
  case sunfish::TTScoreType::Lower:
    os << "Lower";
    break;
  case sunfish::TTScoreType::None:
    os << "None";
    break;
  }
  return os;
}

#endif // SUNFISH_TT_TTSLOT_HPP__
