/* CsaWriter.cpp
 *
 * Kubo Ryosuke
 */

#include "core/record/CsaWriter.hpp"
#include <iomanip>

namespace sunfish {

bool CsaWriter::write(std::ostream& os,
                      const Record& record,
                      const RecordInfo* info /*= nullptr*/) {
  if (info != nullptr) {
    os << "$EVENT:" << info->title << '\n';
    os << "N+" << info->blackName << '\n';
    os << "N-" << info->whiteName << '\n';
    if (info->timeLimitHours != RecordInfo::InvalidTimeValue &&
        info->timeLimitMinutes != RecordInfo::InvalidTimeValue &&
        info->timeLimitReadoff != RecordInfo::InvalidTimeValue) {
    os << "$TIME_LIMIT:" << std::setfill('0') << std::setw(2) << info->timeLimitHours
                  << ":" << std::setfill('0') << std::setw(2) << info->timeLimitMinutes
                  << "+" << std::setfill('0') << std::setw(2) << info->timeLimitReadoff << '\n';
    }
  }

  os << record.initialPosition.toString();

  Position pos = record.initialPosition;
  for (auto& move : record.moveList) {
    os << move.toString(pos) << '\n';
    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "an invalid move is contained.";
      return false;
    }
  }

  return true;
}

} // namespace sunfish
