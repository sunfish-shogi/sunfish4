/* CsaReader.cpp
 *
 * Kubo Ryosuke
 */

#include "core/record/CsaReader.hpp"
#include "common/string/StringUtil.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <string>
#include <cstring>

#define LINE_BUFFER_SIZE 1024

namespace {

using namespace sunfish;

enum InputStatus {
  Continue,
  Break,
  Eof,
  Error,
};

template <class T>
InputStatus forEach(std::istream& is, T&& f) {
  std::string buffer;
  while (true) {
    getline(is, buffer);

    if (is.eof()) {
      return InputStatus::Eof;
    }

    if (is.fail()) {
      LOG(error) << "file I/O error";
      return InputStatus::Error;
    }

    auto lines = StringUtil::split(buffer, ',');

    for (const auto& line : lines) {
      auto status = f(line.c_str());
      if (status != InputStatus::Continue) {
        return status;
      }
    }
  }
}

} // namespace

namespace sunfish {

bool CsaReader::read(std::istream& is,
                     Record& record,
                     RecordInfo* info/* = nullptr*/) {
  MutablePosition mp;
  initializeMutablePosition(mp);

  if (!readPosition(is, mp, info)) {
    return false;
  }

  record.initialPosition.initialize(mp);
  record.moveList.clear();

  Position position = record.initialPosition;

  auto status = forEach(is, [&record, &position, info](const char* line) {
    if (readComment(line) ||
        readTime(line) ||
        readSpecialMove(line, record)) {
      return InputStatus::Continue;
    }

    Move move;
    if (!readMove(line, position, move)) {
      LOG(error) << "invalid move: " << line;
      return InputStatus::Error;
    }

    record.moveList.push_back(move);

    Piece captured;
    if (!position.doMove(move, captured)) {
      LOG(error) << "invalid move: " << line;
      return InputStatus::Error;
    }

    return InputStatus::Continue;
  });

  return status != InputStatus::Error;
}

bool CsaReader::readPosition(std::istream& is,
                             Position& position,
                             RecordInfo* info/* = nullptr*/) {
  MutablePosition mp;
  initializeMutablePosition(mp);

  bool ok = readPosition(is, mp, info);

  if (ok) {
    position.initialize(mp);
  }

  return ok;
}

bool CsaReader::readPosition(std::istream& is,
                             MutablePosition& mp,
                             RecordInfo* info) {
  if (info != nullptr) {
    initializeRecordInfo(*info);
  }

  auto status = forEach(is, [&mp, info](const char* line) {
    if (!readPosition(line, mp, info)) {
      return InputStatus::Error;
    }

    if (line[0] == '+' || line[0] == '-' || (line[0] == 'P' && line[1] == 'I')) {
      return InputStatus::Break;
    }

    return InputStatus::Continue;
  });

  return status != InputStatus::Error;
}

bool CsaReader::readPosition(const char* line,
                             MutablePosition& mp,
                             RecordInfo* info/* = nullptr*/) {
  switch (line[0]) {
  case 'P':
    if (line[1] >= '1' && line[1] <= '9') {
      return readPositionPieces(line, mp);
    } else if (line[1] == 'I') {
      return readPositionI(line, mp);
    } else if (line[1] == '+') {
      return readHand(line, mp, Turn::Black);
    } else if (line[1] == '-') {
      return readHand(line, mp, Turn::White);
    }
    LOG(error) << "invalid format: " << line;
    return false;

  case '+':
    mp.turn = Turn::Black;
    return true;

  case '-':
    mp.turn = Turn::White;
    return true;

  case '$': case 'N':
    return info != nullptr ? readInfo(line, *info) : true;

  case 'V': case '\'': case '\0':
    return true;

  default:
    LOG(error) << "invalid format: " << line;
    return false;
  }
}

bool CsaReader::readPositionPieces(const char* line,
                                   MutablePosition& mp) {
  if (strlen(line) < 2 + 3 * Square::FileMax) {
    LOG(error) << "invalid format: " << line;
    return false;
  }
  int rank = line[1] - '0';
  for (int file = 1; file <= Square::FileMax; file++) {
    Piece piece = Piece::parse(line + 2 + 3 * (9 - file));
    mp.board[Square(file, rank).raw()] = piece;
  }
  return true;
}

bool CsaReader::readPositionI(const char* line,
                              MutablePosition& mp) {
  mp = Position(Position::Handicap::Even).getMutablePosition();

  unsigned length = (unsigned)strlen(line);
  for (unsigned i = 2; i + 4 <= length; i += 4) {
    unsigned file = line[i+0] - '0';
    unsigned rank = line[i+1] - '0';

    if (!Square::isValidFile(file) || !Square::isValidRank(rank)) {
      LOG(error) << "invalid format: " << line;
      return false;
    }

    mp.board[Square(file, rank).raw()] = Piece::empty();
  }

  return true;
}

bool CsaReader::readInfo(const char* line,
                         RecordInfo& info) {
  if (strncmp(line, "$EVENT:", 7) == 0) {
    info.title = &line[7];
    return true;
  }

  if (strncmp(line, "$SITE:", 6) == 0) {
    info.site = &line[6];
    return true;
  }

  if (strncmp(line, "$OPENING:", 9) == 0) {
    info.opening = &line[9];
    return true;
  }

  if (strncmp(line, "$START_TIME:", 12) == 0) {
    info.startTime = &line[12];
    return true;
  }

  if (strncmp(line, "$END_TIME:", 10) == 0) {
    info.endTime = &line[10];
    return true;
  }

  if (strncmp(line, "N+", 2) == 0) {
    info.blackName = &line[2];
    return true;
  }

  if (strncmp(line, "N-", 2) == 0) {
    info.whiteName = &line[2];
    return true;
  }

  if (strncmp(line, "$TIME_LIMIT:", 12) == 0 && strlen(line) >= 20) {
    info.timeLimitHours = std::stoi(&line[12]);
    info.timeLimitMinutes = std::stoi(&line[15]);
    info.timeLimitReadoff = std::stoi(&line[18]);
    return true;
  }

  LOG(error) << "invalid format: " << line;
  return false;
}

bool CsaReader::readHand(const char* line,
                         MutablePosition& mp,
                         Turn turn) {
  unsigned length = (unsigned)strlen(line);

  for (unsigned i = 2; i + 4 <= length; i += 4) {
    unsigned file = line[i+0] - '0';
    unsigned rank = line[i+1] - '0';

    PieceType pieceType = PieceType::parse(&line[i+2]);

    if (pieceType != PieceType::empty()) {

      if (Square::isValidFile(file) && Square::isValidRank(rank)) {
        Piece piece = (turn == Turn::Black) ? pieceType.black() : pieceType.white();
        mp.board[Square(file, rank).raw()] = piece;

      } else if (file == 0 && rank == 0 && !pieceType.isPromoted() && pieceType != PieceType::king()) {
        if (turn == Turn::Black) {
          mp.blackHand.inc(pieceType);
        } else {
          mp.whiteHand.inc(pieceType);
        }

      } else {
        LOG(error) << "invalid format: " << line;
        return false;
      }

    } else {
      LOG(error) << "invalid format: " << line;
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
    // TODO
    return true;
  }
  return false;
}

bool CsaReader::readSpecialMove(const char* line,
                                Record& record) {
  if (line[0] == '%') {
    record.specialMove = line;
    return true;
  }
  return false;
}

bool CsaReader::readMove(const char* line,
                         const Position& position,
                         Move& move) {
  if (strlen(line) < 7) {
    return false;
  }

  bool turn;
  if (line[0] == '+') {
    turn = Turn::Black;
  } else if (line[0] == '-') {
    turn = Turn::White;
  } else {
    return false;
  }

  if (turn != position.getTurn()) {
    return false;
  }

  Square to = Square::parse(&line[3]);
  PieceType pieceType = PieceType::parse(&line[5]);
  if (line[1] == '0' && line[2] == '0') {
    move = Move(pieceType, to);
  } else {
    Square from = Square::parse(&line[1]);
    Piece pieceOrg = position.getPieceOnBoard(from);
    bool promote;
    if (pieceType == pieceOrg.type()) {
      promote = false;
    } else if (pieceType == pieceOrg.type().promote()) {
      promote = true;
    } else {
      return false;
    }
    move = Move(from, to, promote);
  }

  return true;
}

} // namespace sunfish
