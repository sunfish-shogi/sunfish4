/* KifuWriter.cpp
 *
 * Kubo Ryosuke
 */

#include "core/record/KifuWriter.hpp"
#include <iomanip>

namespace sunfish {

namespace {

const char* const KanjiPieceTypes[] = {
  "歩", "香", "桂", "銀", "金", "角", "飛", "玉",
  "と", "杏", "圭", "全", "  ", "馬", "龍", "  ",
};

const char* const KanjiPieces[] = {
  " 歩", " 香", " 桂", " 銀", " 金", " 角", " 飛", " 玉",
  " と", " 杏", " 圭", " 全", "   ", " 馬", " 龍", "   ",
  "v歩", "v香", "v桂", "v銀", "v金", "v角", "v飛", "v玉",
  "vと", "v杏", "v圭", "v全", "   ", "v馬", "v龍", "   ",
};

const char* const Numbers[] = {
  "",
  "１", "２", "３" ,"４" ,"５" ,"６" ,"７" ,"８" ,"９",
};

const char* const KanjiNumbers[] = {
      "",
    "一",   "二",   "三",   "四",   "五",   "六",   "七",   "八",   "九",   "十",
  "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八",
};

} // namespace

bool KifuWriter::write(std::ostream& os,
                      const Record& record,
                      const RecordInfo* info /*= nullptr*/) {
  if (info != nullptr) {
    os << "棋戦：" << info->title << '\n';
    os << "場所：" << info->site << '\n';
    os << "戦型：" << info->opening << '\n';
    os << "開始日時：" << info->startTime << '\n';
    os << "終了日時：" << info->endTime << '\n';
    os << "先手：" << info->blackName << '\n';
    os << "後手：" << info->whiteName << '\n';
    if (info->timeLimitHours != RecordInfo::InvalidTimeValue &&
        info->timeLimitMinutes != RecordInfo::InvalidTimeValue &&
        info->timeLimitReadoff != RecordInfo::InvalidTimeValue) {
    os << "持ち時間：" << std::setfill('0') << std::setw(2) << info->timeLimitHours
                  << ":" << std::setfill('0') << std::setw(2) << info->timeLimitMinutes
                  << "+" << std::setfill('0') << std::setw(2) << info->timeLimitReadoff << '\n';
    }
  }

  {
    os << "後手の持駒：";
    bool empty = true;
    HAND_EACH(pieceType) {
      auto handNum = record.initialPosition.getWhiteHandPieceCount(pieceType);
      if (handNum != 0) {
        os << KanjiPieceTypes[pieceType.raw()] << KanjiNumbers[handNum] << "　";
        empty = false;
      }
    }
    if (empty) {
      os << "なし";
    }
    os << '\n';
  }

  {
    os << "  ９ ８ ７ ６ ５ ４ ３ ２ １\n";
    os << "+---------------------------+\n";
    for (int rank = 1; rank <= 9; rank++) {
      os << 'P' << rank;
      for (int file = 9; file >= 1; file--) {
        auto piece = record.initialPosition.getPieceOnBoard(Square(file, rank));
        if (!piece.isEmpty()) {
          os << KanjiPieces[piece.raw()];
        } else {
          os << " ・";
        }
      }
      os << '\n';
    }
    os << "+---------------------------+\n";
  }

  {
    os << "先手の持駒：";
    bool empty = true;
    HAND_EACH(pieceType) {
      auto handNum = record.initialPosition.getBlackHandPieceCount(pieceType);
      if (handNum != 0) {
        os << KanjiPieceTypes[pieceType.raw()] << KanjiNumbers[handNum] << "　";
        empty = false;
      }
    }
    if (empty) {
      os << "なし";
    }
    os << '\n';
  }


  Position pos = record.initialPosition;
  int idx = 1;
  for (auto& move : record.moveList) {
    os << idx << " ";
    os << Numbers[move.to().getFile()];
    os << KanjiNumbers[move.to().getRank()];
    if (move.isDrop()) {
      os << KanjiPieceTypes[move.droppingPieceType().raw()] << "打";
    } else {
      os << KanjiPieceTypes[pos.getPieceOnBoard(move.from()).type().raw()];
      if (move.isPromotion()) {
        os << "成";
      }
      os << "(" << move.from().getFile() << move.from().getRank() << ")";
    }
    os << '\n';
    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "an invalid move is contained.";
      return false;
    }
    idx++;
  }

  if (record.specialMove == "%TORYO") {
    os << "投了\n";
  } else if (record.specialMove == "%CHUDAN") {
    os << "中断\n";
  } else if (record.specialMove == "%SENNICHITE") {
    os << "千日手\n";
  } else if (record.specialMove == "%TIME_UP") {
    os << "切れ負け\n";
  } else if (record.specialMove == "%ILLEGAL_MOVE") {
    os << "反則負け\n";
  } else if (record.specialMove == "%JISHOGI") {
    os << "持将棋\n";
  }

  return true;
}

} // namespace sunfish
