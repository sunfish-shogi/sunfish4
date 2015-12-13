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
  auto tokens = StringUtil::split(data, [](char c) { return isspace(c); });

  if (tokens.size() < 4) {
    Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
    return false;
  }

  return parsePosition(tokens[0],
                       tokens[1],
                       tokens[2],
                       tokens[3],
                       position);
}

bool SfenParser::parsePosition(const char* arg1,
                               const char* arg2,
                               const char* arg3,
                               const char* /*arg4*/,
                               Position& position) {
  MutablePosition mp;

  {
    int file = 9;
    int rank = 1;
    for (const char* p = arg1; p[0] != '\0'; p++) {
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
    if (strcmp(arg2, "b") == 0) {
      mp.turn = Turn::Black;
    } else if (strcmp(arg2, "w") == 0) {
      mp.turn = Turn::White;
    } else {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }
  }

  {
    HAND_EACH(piece) {
      mp.blackHand.set(piece, 0);
      mp.whiteHand.set(piece, 0);
    }

    if (strcmp(arg3, "-") != 0) {
      for (const char* p = arg3; p[0] != '\0'; p++) {
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

bool SfenParser::parseMove(const char* data, Move& move) {
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

    move = Move(from, to, prom);

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

    move = Move(pieceType, to);
  }

  return true;
}

}
