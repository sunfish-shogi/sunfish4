/* SfenParser.cpp
 *
 * Kubo Ryosuke
 */

#include "common/util/StringUtil.hpp"
#include "core/record/SfenParser.hpp"
#include "logger/Logger.hpp"
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace sunfish {

bool SfenParser::parsePosition(const char* data, Position& position) {
  MutablePosition mp;

  auto tokens = StringUtil::split(data, [](char c) { return isspace(c); });

  if (tokens.size() < 3) {
    Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
    return false;
  }

  {
    const auto& token = tokens[0];
    int file = 9;
    int rank = 1;
    for (const char* p = token.c_str(); p[0] != '\0'; p++) {
      if (p[0] == '/') {
        if (file != 0) {
          Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
          return false;
        }
        if (rank == 9) {
          Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
          return false;
        }
        file = 9;
        rank = rank + 1;

      } else if (isdigit(p[0])) {
        int len = p[0] - '0';
        for (int i = 0; i < len; i++) {
          if (file == 0) {
            Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
            return false;
          }
          mp.board[Square(file, rank).raw()] = Piece::empty();
          file = file - 1;
        }

      } else {
        Piece piece = Piece::parseSFEN(p);
        if (piece.isEmpty()) {
          Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
          return false;
        }
        mp.board[Square(file, rank).raw()] = piece;
        file = file - 1;
        if (p[0] == '+') {
          p++;
        }
      }
    }

    if (file != 0 || rank != 9) {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }
  }

  {
    const auto& token = tokens[1];
    if (token == "b") {
      mp.turn = Turn::Black;
    } else if (token == "w") {
      mp.turn = Turn::White;
    } else {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }
  }

  {
    const auto& token = tokens[2];
    HAND_EACH(piece) {
      mp.blackHand.set(piece, 0);
      mp.whiteHand.set(piece, 0);
    }

    if (token != "-") {
      for (const char* p = token.c_str(); p[0] != '\0'; p++) {
        int num = 1;
        if (isdigit(p[0])) {
          num = strtol(p, nullptr, 10);
          p = isdigit(p[1]) ? p + 2 : p + 1;
        }

        Piece piece = Piece::parseSFEN(p);
        if (piece.isEmpty()) {
          Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
          return false;
        }

        if (piece.isBlack()) {
          mp.blackHand.set(piece.type(), num);
        } else {
          mp.whiteHand.set(piece.type(), num);
        }
      }
    }
  }

  position.initialize(mp);

  return true;
}

bool SfenParser::parseMove(const char* data, const Position& position, Move& move) {
  if (strlen(data) < 4) {
    Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
    return false;
  }

  if (isdigit(data[0])) {
    Square from = Square::parseSFEN(&data[0]);
    Square to = Square::parseSFEN(&data[2]);
    bool prom = data[4] == '+';

    if (!from.isValid() || !to.isValid()) {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    Piece piece = position.getPieceOnBoard(from);

    if (piece.isEmpty()) {
      Loggers::warning << "illegal move: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    move = Move(piece, from, to, prom);

  } else {
    if (data[1] != '*') {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    PieceType pieceType = Piece::parseSFEN(&data[0]).type();
    Square to = Square::parseSFEN(&data[2]);

    if (pieceType.isEmpty() || !to.isValid()) {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    if (position.getTurn() == Turn::Black) {
      move = Move(pieceType.black(), to);
    } else {
      move = Move(pieceType.white(), to);
    }

  }

  return true;
}

}
