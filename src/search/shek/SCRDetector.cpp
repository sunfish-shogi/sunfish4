/* SCRDetector.cpp
 *
 * Kubo Ryosuke
 */

#include "search/shek/SCRDetector.hpp"
#include "search/tree/Tree.hpp"
#include "common/string/StringUtil.hpp"
#include "core/record/Record.hpp"
#include "core/position/Position.hpp"
#include "logger/Logger.hpp"

namespace sunfish {

void SCRDetector::registerRecord(const Record& record) {
  Position pos = record.initialPosition;

  auto recordLength = record.moveList.size();

  for (size_t i = 0; i < recordLength; i++) {
    size_t ri = recordLength - i - 1;
    if (ri < MaxLength) {
      list_[ri].hash = pos.getHash();
      list_[ri].check = pos.inCheck();
    }

    auto& move = record.moveList[i];
    Piece captured;
    if (!pos.doMove(move, captured)) {
      length_ = 0;
      LOG(error) << "illegal move: " << StringUtil::ordinal(i) << ": " << move.toString();
      return;
    }
  }

  // static_cast is required on Clang.
  // See https://trello.com/c/iJqg1GqN
  length_ = std::min(recordLength, static_cast<size_t>(MaxLength));
}

SCRState SCRDetector::detectShort(const Tree& tree) const {
  bool isCurrentPlayerTurn = false;
  bool currentPlayerChecking = true;
  bool enemyPlayerChecking = true;

  for (int ply = tree.ply - 1; ply >= 0; ply--) {
    auto& node = tree.nodes[ply];
    bool check = isCheck(node.checkState);
    if (isCurrentPlayerTurn) {
      enemyPlayerChecking = enemyPlayerChecking && check;
      isCurrentPlayerTurn = false;
    } else {
      currentPlayerChecking = currentPlayerChecking && check;
      isCurrentPlayerTurn = true;
    }

    if (node.hash == tree.position.getHash()) {
      return currentPlayerChecking ? SCRState::Lose :
             enemyPlayerChecking   ? SCRState::Win  :
                                     SCRState::Draw;
    }

    if (!currentPlayerChecking && !enemyPlayerChecking) {
      return SCRState::Draw;
    }
  }

  return SCRState::None;
}

SCRState SCRDetector::detect(const Tree& tree) const {
  int repetitionCount = 0;
  bool isCurrentPlayerTurn = false;
  bool currentPlayerChecking = true;
  bool enemyPlayerChecking = true;

  for (int ply = tree.ply - 1; ply >= 0; ply--) {
    auto& node = tree.nodes[ply];
    bool check = isCheck(node.checkState);
    if (isCurrentPlayerTurn) {
      enemyPlayerChecking = enemyPlayerChecking && check;
      isCurrentPlayerTurn = false;
    } else {
      currentPlayerChecking = currentPlayerChecking && check;
      isCurrentPlayerTurn = true;
    }

    if (node.hash == tree.position.getHash()) {
      repetitionCount++;
      if (repetitionCount == 3) {
        return currentPlayerChecking ? SCRState::Lose :
               enemyPlayerChecking   ? SCRState::Win  :
                                       SCRState::Draw;
      }
    }

    if (!currentPlayerChecking && !enemyPlayerChecking) {
      return SCRState::Draw;
    }
  }

  for (size_t i = 0; i < length_; i++) {
    bool check = list_[i].check;
    if (isCurrentPlayerTurn) {
      enemyPlayerChecking = enemyPlayerChecking && check;
      isCurrentPlayerTurn = false;
    } else {
      currentPlayerChecking = currentPlayerChecking && check;
      isCurrentPlayerTurn = true;
    }

    if (list_[i].hash == tree.position.getHash()) {
      repetitionCount++;
      if (repetitionCount == 3) {
        return currentPlayerChecking ? SCRState::Lose :
               enemyPlayerChecking   ? SCRState::Win  :
                                       SCRState::Draw;
      }
    }

    if (!currentPlayerChecking && !enemyPlayerChecking) {
      return SCRState::Draw;
    }
  }

  return SCRState::None;
}

} // namespace sunfish
