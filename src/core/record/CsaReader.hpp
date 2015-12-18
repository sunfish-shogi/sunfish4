/* CsaReader.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_CSAREADER_HPP__
#define SUNFISH_CORE_RECORD_CSAREADER_HPP__

#include "core/base/Turn.hpp"
#include "core/position/Position.hpp"
#include "core/move/Move.hpp"
#include "core/record/Record.hpp"
#include <iostream>
#include <array>

namespace sunfish {

class CsaReader {
public:

  static bool read(std::istream& is, Record& record, RecordInfo* info = nullptr);
  static bool readPosition(std::istream& is, Position& position, RecordInfo* info = nullptr);
  static bool readPosition(const char* line, MutablePosition& position, RecordInfo* info = nullptr);
  static bool readMove(const char* line, const Position& position, Move& move);

private:

  CsaReader();

  static bool readPosition(std::istream& is, MutablePosition& mp, RecordInfo* info);
  static bool readPositionPieces(const char* line, MutablePosition& mp);
  static bool readInfo(const char* line, RecordInfo& info);
  static bool readHand(const char* line, MutablePosition& position, Turn turn);
  static bool readComment(const char* line);
  static bool readTime(const char* line);
  static bool readCommand(const char* line);

};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_CSAREADER_HPP__
