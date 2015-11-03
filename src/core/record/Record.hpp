/* Record.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_RECORD__
#define SUNFISH_CORE_RECORD_RECORD__

#include <string>

namespace sunfish {

/**
 * Information of the record
 */
struct RecordInfo {
  /** the title of the record */
  std::string title;

  /** the name of black player */
  std::string blackName;

  /** the name of white player */
  std::string whiteName;

  /** hour-part of the time limit */
  int timeLimitHour;

  /** minutes-part of the time limit */
  int timeLimitMinutes;

  /** readoff-part of the time limit */
  int timeLimitReadoff;
};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_RECORD__
