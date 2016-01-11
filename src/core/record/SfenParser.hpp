/* SfenParser.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_SFENPARSER_HPP__
#define SUNFISH_CORE_RECORD_SFENPARSER_HPP__

#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include "core/record/Record.hpp"

namespace sunfish {

class SfenParser {
public:

  static bool parsePosition(const char* data, Position& position);

  static bool parsePosition(const std::string& data, Position& position) {
    return parsePosition(data.c_str(), position);
  }

  static bool parsePosition(const char* arg1,
                            const char* arg2,
                            const char* arg3,
                            const char* arg4,
                            Position& position);

  static bool parsePosition(const std::string& arg1,
                            const std::string& arg2,
                            const std::string& arg3,
                            const std::string& arg4,
                            Position& position) {
    return parsePosition(arg1.c_str(),
                         arg2.c_str(),
                         arg3.c_str(),
                         arg4.c_str(),
                         position);
  }

  static bool parseMove(const char* data, Move& move);

  static bool parseMove(const std::string& data, Move& move) {
    return parseMove(data.c_str(), move);
  }

  template <class iterator>
  static bool parseUsiCommand(const iterator& begin,
                              const iterator& end,
                              Record& record);

private:

  SfenParser();

};

template <class iterator>
inline
bool SfenParser::parseUsiCommand(const iterator& begin,
                                 const iterator& end,
                                 Record& record) {
  iterator ite = begin;

  if (ite == end || *ite != "position") {
    return false;
  }
  ite++;

  if (ite != end && *ite == "startpos") {
    record.initialPosition.initialize(Position::Handicap::Even);
    ite++;
  } else if (ite != end && *ite == "sfen") {
    auto& arg1 = *(ite++); if (ite == end) { return false; }
    auto& arg2 = *(ite++); if (ite == end) { return false; }
    auto& arg3 = *(ite++); if (ite == end) { return false; }
    auto& arg4 = *(ite++); if (ite == end) { return false; }
    bool ok = parsePosition(arg1,
                            arg2,
                            arg3,
                            arg4,
                            record.initialPosition);
    if (!ok) {
      return false;
    }
  } else {
    return false;
  }

  if (ite == end) {
    return true;
  }

  if (*ite != "moves") {
    return false;
  }
  ite++;

  for (; ite != end; ite++) {
    Move move;
    if (!parseMove(*ite, move)) {
      return false;
    }
    record.moveList.push_back(move);
  }

  return true;
}

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_SFENPARSER_HPP__
