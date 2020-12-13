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
#include <climits>

namespace sunfish {

using HistoryValue = int16_t;
static CONSTEXPR_CONST int16_t HistoryMax = INT16_MAX / 3;
static CONSTEXPR_CONST int16_t HistoryScale = 32;

template <int Size>
class History {
public:

  History() {
    clear();
  }

  void clear() {
    memset((void*)hist_, 0, sizeof(hist_));
  }

  void reduce() {
    for (int i = 0; i < Size; i++) {
      hist_[i] = hist_[i] / 2;
    }
  }

protected:

  void updateByIndex(int index, HistoryValue value) {
    HistoryValue reduce = int32_t(hist_[index]) * (int32_t(value) + 1) / int32_t(HistoryMax / HistoryScale);
    if (value >= 0) {
      hist_[index] -= reduce;
    } else {
      hist_[index] += reduce;
    }
    hist_[index] += HistoryScale * value;
  }

  HistoryValue getByIndex(int index) const {
    return hist_[index];
  }

private:

  HistoryValue hist_[Size];

};

class FromToHistory : public History<Square::N * Square::N * 2> {
public:

  void update(Turn turn, Square from, Square to, HistoryValue value) {
    updateByIndex(index(turn, from, to), value);
  }

  HistoryValue get(Turn turn, Square from, Square to) const {
    return getByIndex(index(turn, from, to));
  }

private:

  static int index(Turn turn, Square from, Square to) {
    return from.raw() * Square::N + to.raw()
         + (turn == Turn::Black ? 0 : Square::N * Square::N);
  }
};

class PieceToHistory : public History<Square::N * PieceNumber::Num * 2> {
public:

  void update(Turn turn, PieceType pieceType, Square to, HistoryValue value) {
    updateByIndex(index(turn, pieceType, to), value);
  }

  HistoryValue get(Turn turn, PieceType pieceType, Square to) const {
    return getByIndex(index(turn, pieceType, to));
  }

private:

  static int index(Turn turn, PieceType pieceType, Square to) {
    return pieceType.raw() * Square::N + to.raw()
         + (turn == Turn::Black ? 0 : Square::N * PieceNumber::Num);
  }
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_HISTORY_HISTORY_HPP__
