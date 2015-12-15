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
#define TT_VALUE_WIDTH 16 // 2^16
#define TT_VTYPE_WIDTH 2  // 2^2 = 4
#define TT_DEPTH_WIDTH 10 // 2^10 = 1024
#define TT_CSUM_WIDTH  16

#define TT_MOVE_SHIFT  0
#define TT_VALUE_SHIFT (TT_MOVE_SHIFT + TT_MOVE_WIDTH)
#define TT_VTYPE_SHIFT (TT_VALUE_SHIFT + TT_VALUE_WIDTH)
#define TT_DEPTH_SHIFT (TT_VTYPE_SHIFT + TT_VTYPE_WIDTH)

#define TT_MOVE_MASK   (((1LLU << TT_MOVE_WIDTH) - 1) << TT_MOVE_SHIFT)
#define TT_VALUE_MASK  (((1LLU << TT_VALUE_WIDTH) - 1) << TT_VALUE_SHIFT)
#define TT_VTYPE_MASK  (((1LLU << TT_VTYPE_WIDTH) - 1) << TT_VTYPE_SHIFT)
#define TT_DEPTH_MASK  (((1LLU << TT_DEPTH_WIDTH) - 1) << TT_DEPTH_SHIFT)
#define TT_CSUM_MASK   (~((1LLU << (64 - TT_CSUM_WIDTH)) - 1))

static_assert(sizeof(sunfish::Score::RawType) == 2, "invalid data size");

static_assert(TT_MOVE_WIDTH
            + TT_VALUE_WIDTH
            + TT_VTYPE_WIDTH
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

  static CONSTEXPR_CONST AgeType InvalidAge = 0x00;
  static CONSTEXPR_CONST AgeType AgeMax = 0x01 << TT_AGE_WIDTH;

private:

  QuadWord _1;
  QuadWord _2;

  bool update(Zobrist::Type newHash,
              Score newScore,
              TTScoreType newScoreType,
              int newDepth,
              int ply,
              Move move,
              AgeType newAge);

  QuadWord calcCheckSum() const {
    static_assert(TT_CSUM_WIDTH == 16, "invalid data size");
    static_assert(TT_CSUM_MASK == 0xffff000000000000LLU, "invalid data size");
    return TT_CSUM_MASK &
      (
        (_1) ^
        (_1 << 16) ^
        (_1 << 32) ^
        (_1 << 48) ^
        (_2 << 16) ^
        (_2 << 32) ^
        (_2 << 48)
      );
  }

public:
  TTElement() {
    clear();
  }

  void clear() {
    assert((InvalidAge << TT_AGE_SHIFT) < TT_AGE_MASK);
    _1 = InvalidAge << TT_AGE_SHIFT;
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
    return ((_1 ^ hash          ) & TT_HASH_MASK) == 0LLU
        && ((_2 ^ calcCheckSum()) & TT_CSUM_MASK) == 0LLU;
  }

  Zobrist::Type hash() const {
    return _1 & TT_HASH_MASK;
  }

  Score score(int ply) const {
    auto data = (_2 & TT_VALUE_MASK) >> TT_VALUE_SHIFT;
    auto rawValue = static_cast<Score::RawType>(data);
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
    auto data = (_2 & TT_VTYPE_MASK) >> TT_VTYPE_SHIFT;
    return static_cast<TTScoreType>(data);
  }

  int depth() const {
    auto data = (_2 & TT_DEPTH_MASK) >> TT_DEPTH_SHIFT;
    return static_cast<int>(data);
  }

  Move move() const {
    auto data = (_2 & TT_MOVE_MASK) >> TT_MOVE_SHIFT;
    auto rawValue = static_cast<Move::RawType16>(data);
    return Move::deserialize(rawValue);
  }

  AgeType age() const {
    auto data = (_1 & TT_AGE_MASK) >> TT_AGE_SHIFT;
    return static_cast<AgeType>(data);
  }

};

} // namespace sunfish

#endif // SUNFISH_TT_TTSLOT_HPP__
