/* Record.cpp
 *
 * Kubo Ryosuke
 */

#include "core/record/Record.hpp"
#include "common/string/StringUtil.hpp"

namespace sunfish {

Position generatePosition(const Record& record,
                          int numberOfMoves) {
  Position pos = record.initialPosition;

  if (numberOfMoves < 0 || static_cast<size_t>(numberOfMoves) > record.moveList.size()) {
    numberOfMoves = record.moveList.size();
  }

  for (int i = 0; i < numberOfMoves; i++) {
    auto& move = record.moveList[i];
    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "fatal error: an illegal move is contained: "
                 << StringUtil::ordinal << ": " << move.toString();
      break;
    }
  }

  return pos;
}

} // namespace sunfish
