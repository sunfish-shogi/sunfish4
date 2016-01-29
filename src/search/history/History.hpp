/* History.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_HISTORY_HISTORY_HPP__
#define SUNFISH_SEARCH_HISTORY_HISTORY_HPP__

#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include "core/move/Move.hpp"
#include "logger/Logger.hpp"
#include <cstdint>
#include <cstring>

namespace sunfish {

class History {
public:

  using RawType = uint64_t;
  using CountType = uint32_t;

  static CONSTEXPR_CONST int From = Square::N + PieceNumber::HandNum * 2;
  static CONSTEXPR_CONST int To = Square::N;
  static CONSTEXPR_CONST int Size = From * To;

  static CONSTEXPR_CONST RawType AppearMask = 0xffffffff00000000ULL;
  static CONSTEXPR_CONST RawType GoodMask   = 0x00000000ffffffffULL;
  static CONSTEXPR_CONST int AppearShift    = 32;
  static CONSTEXPR_CONST RawType Max        = 0x0007ffff0007ffffull;
  static CONSTEXPR_CONST CountType Scale    = 0x2000;

  struct Data {
    CountType good;
    CountType appear;
  };

  History() {
    clear();
  }

  void clear() {
    memset((void*)hist_, 0, sizeof(hist_));
  }

  void reduce() {
    for (int i = 0; i < Size; i++) {
      hist_[i] = (hist_[i] >> 8) & ~((AppearMask >> 8) & GoodMask);
    }
  }

  void add(Turn turn, const Move& move, CountType appear, CountType good) {
    ASSERT(good <= appear);
    int k = key(turn, move);
    RawType h = hist_[k];
    RawType d = ((uint64_t)appear << 32) + good;
    if (h > Max - d) {
      h = (h >> 1) & ~0x80000000ull;
    }
    hist_[k] = h + d;
  }

  Data get(Turn turn, const Move& move) const {
    RawType h = hist_[key(turn, move)];
    return {
      static_cast<CountType>(h),
      static_cast<CountType>(h >> AppearShift)
    };
  }

  CountType ratio(Turn turn, const Move& move) const {
    Data data = get(turn, move);
    ASSERT(data.appear >= data.good);
    return (data.good + 1) * Scale / (data.appear + 2);
  }

private:

  static int from(Turn turn, const Move& move) {
    if (move.isDrop()) {
      auto raw = move.droppingPieceType().raw();
      return Square::N + raw + (turn == Turn::Black ? 0 : PieceNumber::HandNum);
    } else {
      return move.from().raw();
    }
  }

  static int to(const Move& move) {
    return move.to().raw();
  }

  static int key(Turn turn, const Move& move) {
    int f = from(turn, move);
    int t = to(move);
    return f * To + t;
  }

  RawType hist_[Size];

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_HISTORY_HISTORY_HPP__
