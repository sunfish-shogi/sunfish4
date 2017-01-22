/* MoveGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__
#define SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__

#include "core/move/Moves.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

class MoveGenerator {
public:

  /**
   * Generate capturing moves.
   * The result includes the illegal moves which leave check.
   */
  static void generateCaptures(const Position& pos, Moves& moves) {
    ASSERT(!pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::Capture, false>(pos, moves, Bitboard::full());
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::Capture, false>(pos, moves, Bitboard::full());
    }
  }

  /**
   * Generate not-capturing moves.
   * The result includes the illegal moves which leave check.
   */
  static void generateQuiets(const Position& pos, Moves& moves) {
    ASSERT(!pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::Quiet, false>(pos, moves, Bitboard::full());
      generateDrops<Turn::Black>(pos, moves, Bitboard::full());
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::Quiet, false>(pos, moves, Bitboard::full());
      generateDrops<Turn::White>(pos, moves, Bitboard::full());
    }
  }

  /**
   * Generate evasions.
   * The result includes the illegal moves which leave check.
   */
  static void generateEvasions(const Position& pos, CheckState checkState, Moves& moves) {
    ASSERT(pos.inCheck());
    if (pos.getTurn() == Turn::Black) {
      generateEvasions<Turn::Black>(pos, checkState, moves);
    } else {
      generateEvasions<Turn::White>(pos, checkState, moves);
    }
  }

private:

  MoveGenerator();

  enum class GenerationType {
    Capture,
    Quiet,
    All,
  };

  template <Turn turn, GenerationType type, bool exceptKing>
  static void generateMovesOnBoard(const Position& pos, Moves& moves, const Bitboard& mask);

  template <Turn turn>
  static void generateDrops(const Position& pos, Moves& moves, const Bitboard& mask);

  template <Turn turn>
  static void generateEvasions(const Position& pos, CheckState checkState, Moves& moves);

};

inline bool isTacticalMove(const Position& position, const Move& move) {
  if (move.isPromotion() &&
      position.getPieceOnBoard(move.from()).type() != PieceType::silver()) {
    return true;
  }

  if (position.isCapture(move)) {
    return true;
  }

  return false;
}

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__
