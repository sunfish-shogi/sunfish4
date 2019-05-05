/* Record.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_RECORD__
#define SUNFISH_CORE_RECORD_RECORD__

#include "core/position/Position.hpp"
#include "core/move/Move.hpp"
#include <string>
#include <vector>
#include <utility>

namespace sunfish {

/**
 * Information of the record
 */
struct RecordInfo {
  static CONSTEXPR_CONST int InvalidTimeValue = -1;

  /** the title of the record */
  std::string title;

  std::string site;

  std::string opening;

  std::string startTime;

  std::string endTime;

  /** the name of black player */
  std::string blackName;

  /** the name of white player */
  std::string whiteName;

  /** hour-part of the time limit */
  int timeLimitHours;

  /** minutes-part of the time limit */
  int timeLimitMinutes;

  /** readoff-part of the time limit */
  int timeLimitReadoff;
};

inline
void initializeRecordInfo(RecordInfo& info) {
  info.title = "";
  info.site = "";
  info.opening = "";
  info.startTime = "";
  info.endTime = "";
  info.blackName = "";
  info.whiteName = "";
  info.timeLimitHours = RecordInfo::InvalidTimeValue;
  info.timeLimitMinutes = RecordInfo::InvalidTimeValue;
  info.timeLimitReadoff = RecordInfo::InvalidTimeValue;
}

struct Record {
  using MoveListType = std::vector<Move>;

  Position initialPosition;
  MoveListType moveList;
  std::string specialMove;
};

/**
 * if numberOfMoves is a negative number, returns the last position.
 */
Position generatePosition(const Record& record,
                          int numberOfMoves);

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_RECORD__
