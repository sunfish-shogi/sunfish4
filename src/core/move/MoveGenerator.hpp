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

  static void generateCapturingMoves(const Position& pos, Moves& moves) {
    assert(!pos.isChecking());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::Capturing>(pos, moves);
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::Capturing>(pos, moves);
    }
  }

  static void generateNotCapturingMoves(const Position& pos, Moves& moves) {
    assert(!pos.isChecking());
    if (pos.getTurn() == Turn::Black) {
      generateMovesOnBoard<Turn::Black, GenerationType::NotCapturing>(pos, moves);
      generateDrops<Turn::Black>(pos, moves);
    } else {
      generateMovesOnBoard<Turn::White, GenerationType::NotCapturing>(pos, moves);
      generateDrops<Turn::White>(pos, moves);
    }
  }

  static void generateEvasions(const Position& pos, Moves& moves) {
    assert(pos.isChecking());
    if (pos.getTurn() == Turn::Black) {
      generateEvasions<Turn::Black>(pos, moves);
    } else {
      generateEvasions<Turn::White>(pos, moves);
    }
  }

private:

  MoveGenerator();

  enum class GenerationType {
    Capturing,
    NotCapturing,
  };

  template <Turn turn, GenerationType type>
  static void generateMovesOnBoard(const Position& pos, Moves& moves);

  template <Turn turn>
  static void generateDrops(const Position& pos, Moves& moves);

  template <Turn turn>
  static void generateEvasions(const Position& pos, Moves& moves);

};

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVEGENERATOR_HPP__
