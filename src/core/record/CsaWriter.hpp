/* CsaWriter.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_CSAWRITER_HPP__
#define SUNFISH_CORE_RECORD_CSAWRITER_HPP__

#include "core/record/Record.hpp"

namespace sunfish {

class CsaWriter {
public:

  CsaWriter() = delete;

  static bool write(std::ostream& os,
                    const Record& record,
                    const RecordInfo* info = nullptr);

};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_CSAWRITER_HPP__
