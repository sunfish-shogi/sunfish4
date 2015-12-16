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
#define TT_AGE_WIDTH   3  // 2^3 = 8
#define TT_HASH_WIDTH  61

#define TT_AGE_SHIFT   0
#define TT_MATE_SHIFT  (TT_AGE_SHIFT + TT_AGE_WIDTH)

#define TT_AGE_MASK    (((1LLU << TT_AGE_WIDTH) - 1LLU) << TT_AGE_SHIFT)
#define TT_HASH_MASK   (~((1LLU << (64 - TT_HASH_WIDTH)) - 1))

static_assert(TT_AGE_WIDTH
            + TT_HASH_WIDTH <= 64, "invalid data size");

// 2nd quad word
#define TT_MOVE_WIDTH  16
#define TT_SCORE_WIDTH 16 // 2^16
#define TT_STYPE_WIDTH 2  // 2^2 = 4
#define TT_DEPTH_WIDTH 10 // 2^10 = 1024
#define TT_CSUM_WIDTH  16

#define TT_MOVE_SHIFT  0
#define TT_SCORE_SHIFT (TT_MOVE_SHIFT + TT_MOVE_WIDTH)
#define TT_STYPE_SHIFT (TT_SCORE_SHIFT + TT_SCORE_WIDTH)
#define TT_DEPTH_SHIFT (TT_STYPE_SHIFT + TT_STYPE_WIDTH)

#define TT_MOVE_MASK   (((1LLU << TT_MOVE_WIDTH) - 1) << TT_MOVE_SHIFT)
#define TT_SCORE_MASK  (((1LLU << TT_SCORE_WIDTH) - 1) << TT_SCORE_SHIFT)
#define TT_STYPE_MASK  (((1LLU << TT_STYPE_WIDTH) - 1) << TT_STYPE_SHIFT)
#define TT_DEPTH_MASK  (((1LLU << TT_DEPTH_WIDTH) - 1) << TT_DEPTH_SHIFT)
#define TT_CSUM_MASK   (~((1LLU << (64 - TT_CSUM_WIDTH)) - 1))

static_assert(sizeof(sunfish::Score::RawType) == 2, "invalid data size");

static_assert(TT_CSUM_WIDTH == 16, "invalid data size");
static_assert(TT_CSUM_MASK == 0xffff000000000000LLU, "invalid data size");

static_assert(TT_MOVE_WIDTH
            + TT_SCORE_WIDTH
            + TT_STYPE_WIDTH
            + TT_DEPTH_WIDTH
            + TT_CSUM_WIDTH <= 64, "invalid data size");

namespace sunfish {

enum class TTStatus : int {
  None,
  Reject,
  New,
  Update,
  Collide,
};

enum TTScoreType : int {
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
  TTElement() {
    clear();
  }

  void clear() {
    w1_ = 0;
    w2_ = ~(calcCheckSum() & TT_CSUM_MASK);
  }

  bool update(Zobrist::Type newHash,
      Score alpha,
      Score beta,
      Score newScore,
      int newDepth, int ply,
      const Move& move,
      AgeType newAge) {
    TTScoreType newScoreType;
    if (newScore >= beta) {
      newScoreType = Lower;
    } else if (newScore <= alpha) {
      newScoreType = Upper;
    } else {
      newScoreType = Exact;
    }

    return update(newHash,
                  newScore,
                  newScoreType,
                  newDepth,
                  ply,
                  move,
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
    assert(st == TTScoreType::None || s >= -Score::infinity());
    assert(st == TTScoreType::None || s <= Score::infinity());

    if (s >= Score::mate()) {
      if (st == Lower) { return s - ply; }
    } else if (s <= -Score::mate()) {
      if (st == Upper) { return s + ply; }
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

};

} // namespace sunfish

#endif // SUNFISH_TT_TTSLOT_HPP__
