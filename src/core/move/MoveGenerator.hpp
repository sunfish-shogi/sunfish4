/* MoveGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__
#define SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__

#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"

namespace sunfish {

class MoveGenerator {
public:

  /**
   * Generate capturing moves.
   * The result includes the illegal moves which leave check.
   */
  static void generateCapturingMoves(const Position& pos, Moves& moves) {
    assert(!pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::Capturing, false>(pos, moves, Bitboard::full());
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::Capturing, false>(pos, moves, Bitboard::full());
    }
  }

  /**
   * Generate not-capturing moves.
   * The result includes the illegal moves which leave check.
   */
  static void generateNotCapturingMoves(const Position& pos, Moves& moves) {
    assert(!pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::NotCapturing, false>(pos, moves, Bitboard::full());
      generateDrops<Turn::Black>(pos, moves, Bitboard::full());
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::NotCapturing, false>(pos, moves, Bitboard::full());
      generateDrops<Turn::White>(pos, moves, Bitboard::full());
    }
  }

  /**
   * Generate evasions.
   * The result includes the illegal moves which leave check.
   */
  static void generateEvasions(const Position& pos, CheckState checkState, Moves& moves) {
    assert(pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateEvasions<Turn::Black>(pos, checkState, moves);
    } else {
      generateEvasions<Turn::White>(pos, checkState, moves);
    }
  }

private:

  MoveGenerator();

  enum class GenerationType {
    Capturing,
    NotCapturing,
    All,
  };

  template <Turn turn, GenerationType type, bool exceptKing>
  static void generateMovesOnBoard(const Position& pos, Moves& moves, const Bitboard& mask);

  template <Turn turn>
  static void generateDrops(const Position& pos, Moves& moves, const Bitboard& mask);

  template <Turn turn>
  static void generateEvasions(const Position& pos, CheckState checkState, Moves& moves);

};

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__
