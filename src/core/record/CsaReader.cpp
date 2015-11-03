/* CsaReader.cpp
 *
 * Kubo Ryosuke
 */

#include "core/record/CsaReader.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <string>
#include <cstring>

#define LINE_BUFFER_SIZE 1024

namespace sunfish {

void CsaReader::initializeMutablePosition(MutablePosition& mp) {
  SQUARE_EACH(square) {
    mp.board[square.raw()] = Piece::empty();
  }

  HAND_EACH(piece) {
    mp.blackHand.set(piece, 0);
    mp.whiteHand.set(piece, 0);
  }

  mp.blackTurn = true;
}

bool CsaReader::readPosition(std::istream& is, Position& position, RecordInfo* info/* = nullptr*/) {
  MutablePosition mp;
  initializeMutablePosition(mp);

  bool ok = readPosition(is, mp, info);

  if (ok) {
    position.initialize(mp.board, mp.blackHand, mp.whiteHand, mp.blackTurn);
  }

  return ok;
}

bool CsaReader::readPosition(const char* line, Position& position, RecordInfo* info/* = nullptr*/) {
  MutablePosition mp;
  initializeMutablePosition(mp);

  bool ok = readPosition(line, mp, info);

  if (ok) {
    position.initialize(mp.board, mp.blackHand, mp.whiteHand, mp.blackTurn);
  }

  return ok;
}

bool CsaReader::readPosition(std::istream& is, MutablePosition& mp, RecordInfo* info/* = nullptr*/) {
  char line[LINE_BUFFER_SIZE];

  while (true) {
    is.getline(line, sizeof(line));

    if (is.eof()) { break; }

    if (is.fail()) {
      Loggers::warning << "file io error: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    if (!readPosition(line, mp, info)) {
      Loggers::warning << "invalid position format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }

    if (line[0] == '+' || line[0] == '-') {
      break;
    }
  }

  return true;
}

bool CsaReader::readPosition(const char* line, MutablePosition& mp, RecordInfo* info/* = nullptr*/) {
  switch (line[0]) {
  case 'P':
    if (line[1] >= '1' && line[1] <= '9') {
      return readPositionPieces(line, mp);
    } else if (line[1] == '+') {
      return readHand(line, mp, true);
    } else if (line[1] == '-') {
      return readHand(line, mp, false);
    }
    Loggers::warning << __FILE_LINE__ << ": unknown command";
    Loggers::warning << line;
    return false;

  case '+':
    mp.blackTurn = true;
    return true;

  case '-':
    mp.blackTurn = false;
    return true;

  case '$': case 'N':
    return info != nullptr ? readInfo(line, *info) : true;

  case 'V': case '\'': case '\0':
    return true;

  default:
    Loggers::warning << __FILE_LINE__ << ": unknown command";
    Loggers::warning << line;
    return false;
  }
}

bool CsaReader::readPositionPieces(const char* line, MutablePosition& mp) {
  if (strlen(line) < 2 + 3 * Square::FileMax) {
    Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
    return false;
  }
  int rank = line[1] - '0';
  for (int file = 1; file <= Square::FileMax; file++) {
    Piece piece = Piece::parse(line + 2 + 3 * (9 - file));
    mp.board[Square(file, rank).raw()] = piece;
  }
  return true;
}

bool CsaReader::readInfo(const char* line, RecordInfo& info) {
  if (strncmp(line, "$EVENT:", 7) == 0) {
    info.title = &line[7];

  } else if (strncmp(line, "N+", 2) == 0) {
    info.blackName = &line[2];

  } else if (strncmp(line, "N-", 2) == 0) {
    info.whiteName = &line[2];

  } else if (strncmp(line, "$TIME_LIMIT:", 12) == 0 && strlen(line) >= 20) {
    info.timeLimitHour = std::stoi(&line[12]);
    info.timeLimitMinutes = std::stoi(&line[15]);
    info.timeLimitReadoff = std::stoi(&line[18]);

  } else {
    Loggers::warning << "unknown command: [" << line << "]: " << __FILE__ << "(" << __LINE__ << ")";

  }

  return true;
}

bool CsaReader::readHand(const char* line, MutablePosition& mp, bool black) {
  unsigned length = (unsigned)strlen(line);

  for (unsigned i = 2; i + 4 <= length; i += 4) {
    unsigned file = line[i+0] - '0';
    unsigned rank = line[i+1] - '0';

    PieceType pieceType = PieceType::parse(&line[i+2]);

    if (pieceType != PieceType::empty()) {

      if (Square::isValidFile(file) && Square::isValidRank(rank)) {
        Piece piece = black ? pieceType.black() : pieceType.white();
        mp.board[Square(file, rank).raw()] = piece;

      } else if (file == 0 && rank == 0 && pieceType.isUnpromoted() && pieceType != PieceType::king()) {
        if (black) {
          mp.blackHand.inc(pieceType);
        } else {
          mp.whiteHand.inc(pieceType);
        }

      } else {
        Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
        return false;
      }

    } else {
      Loggers::warning << "invalid format: " << __FILE__ << "(" << __LINE__ << ")";
      return false;
    }
  }

  return true;
}

bool CsaReader::readComment(const char* line) {
  if (line[0] == '\0' || line[0] == '\'') {
    return true;
  }
  return false;
}

bool CsaReader::readTime(const char* line) {
  if (line[0] == 'T') {
    return true;
  }
  return false;
}

bool CsaReader::readCommand(const char* line) {
  if (line[0] == '%') {
    return true;
  }
  return false;
}

bool CsaReader::readMove(const char* line, const Position& position, Move& move) {
  if (strlen(line) < 7) {
    return false;
  }

  bool black;
  if (line[0] == '+') {
    black = true;
  } else if (line[0] == '-') {
    black = false;
  } else {
    return false;
  }

  if (black != position.isBlackTurn()) {
    return false;
  }

  Square to = Square::parse(&line[3]);
  PieceType pieceType = PieceType::parse(&line[5]);
  Piece piece = black ? pieceType.black() : pieceType.white();
  if (line[1] == '0' && line[2] == '0') {
    move = Move(piece, to);
  } else {
    Square from = Square::parse(&line[1]);
    Piece pieceOrg = position.getPieceOnBoard(from);
    bool promote;
    if (piece == pieceOrg) {
      promote = false;
    } else if (piece == pieceOrg.promote()) {
      promote = true;
    } else {
      return false;
    }
    move = Move(pieceOrg, from, to, promote);
  }

  return true;
}

} // namespace sunfish
