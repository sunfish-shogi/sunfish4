/* MoveGenerationTest.cpp
 *
 * Kubo Ryosuke
 */

#include "expt/mgtest/MoveGenerationTest.hpp"
#include "expt/mgtest/TardyMoveGenerator.hpp"
#include "common/string/StringUtil.hpp"
#include "common/math/Random.hpp"
#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"

namespace sunfish {

bool MoveGenerationTest::test(int repeatCount, int maximumNumberOfMoves) {
  for (int count = 1; count <= repeatCount; count++) {
    if (!testOnce(count, maximumNumberOfMoves)) {
      return false;
    }
  }
  return true;
}

bool MoveGenerationTest::testOnce(int count, int maximumNumberOfMoves) {
  OUT(info) << "Test Count: " << count;

  Position position(Position::Handicap::Even);

  for (int nom = 1; nom <= maximumNumberOfMoves; nom++) {
    auto status = testAndDoMove(position);

    if (status == TestStatus::Error) {
      OUT(info) << "Stopped on " << StringUtil::ordinal(nom) << " move.";
      return false;
    }

    if (status == TestStatus::Mate) {
      OUT(info) << "Mate on " << StringUtil::ordinal(nom) << " move.";
      return true;
    }
  }

  OUT(info) << "Reached to the maximum number of moves.";
  return true;
}

MoveGenerationTest::TestStatus MoveGenerationTest::testAndDoMove(Position& position) {
  CheckState cs = position.getCheckState();

  Moves moves;
  if (!isCheck(cs)) {
    MoveGenerator::generateCaptures(position, moves);
    MoveGenerator::generateQuiets(position, moves);
  } else {
    MoveGenerator::generateEvasions(position, cs, moves);
  }

  Moves moves2;
  if (!isCheck(cs)) {
    TardyMoveGenerator::generateMoves(position, moves2);
  } else {
    TardyMoveGenerator::generateEvasions(position, cs, moves2);
  }

  sortMovesForDebug(moves, position);
  sortMovesForDebug(moves2, position);

  if (moves.size() != moves2.size()) {
    OUT(error) << "sizes of moves are not euqal.";
    OUT(error) << position.toString();
    OUT(error) << "moves 1: "
               << "n=" << moves.size() << ": "
               << moves.toString(position);
    OUT(error) << "moves 2: "
               << "n=" << moves2.size() << ": "
               << moves2.toString(position);
    return TestStatus::Error;
  }

  for (Moves::size_type i = 0; i < moves.size(); i++) {
    if (moves[i] != moves2[i]) {
      OUT(error) << "generated different moves.";
      OUT(error) << "moves 1: "
                 << "n=" << moves.size() << ": "
                 << moves.toString(position);
      OUT(error) << "moves 2: "
                 << "n=" << moves2.size() << ": "
                 << moves2.toString(position);
      OUT(error) << moves[i].toString(position)
                 << " is not equal to "
                 << moves2[i].toString(position);
      return TestStatus::Error;
    }
  }

  // exclude illegal moves
  for (auto ite = moves.begin(); ite != moves.end(); ) {
    Move move = *ite;
    Piece captured;
    if (position.doMove(move, captured)) {
      position.undoMove(move, captured);
      ite++;
    } else {
      ite = moves.remove(ite);
    }
  }

  if (moves.size() == 0) {
    return TestStatus::Mate;
  }

  auto r = Random::uint16(moves.size());
  auto move = moves[r];

  Piece captured;
  if (!position.doMove(move, captured)) {
    LOG(error) << "fatal error";
    OUT(error) << position.toString();
    OUT(error) << move.toString(position);
    return TestStatus::Error;
  }

  return TestStatus::Continue;
}

} // namespace sunfish
