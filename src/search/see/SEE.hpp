/* SEE.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEE_SEE_HPP__
#define SUNFISH_SEARCH_SEE_SEE_HPP__

#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"
#include "core/move/Move.hpp"
#include "search/eval/Score.hpp"

void test_method_ScoreTest_testGenerateAggressors();

namespace sunfish {

class SEE {
public:

  struct Aggressor {
    PieceType piece;
    Square square;
    Score prom;
    Score exch;
  };

  SEE() = delete;

  static Score calculate(const Position& position,
                         Move move);

  static Bitboard extractAggressors(const Position& position,
                                    Square from,
                                    Square to);

  static Bitboard extractShadowAggressor(const Position& position,
                                         Bitboard bb,
                                         Square from,
                                         Square to);

private:

  static Score search(const Position& position,
                      Bitboard bb,
                      Square to,
                      Score score,
                      Score materialScore);

  template <Turn turn>
  static Aggressor pickAggressor(const Position& position,
                                Bitboard& bb,
                                Square to);

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEE_SEE_HPP__
