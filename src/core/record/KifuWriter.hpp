/* KifuWriter.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_KIFUWRITER_HPP__
#define SUNFISH_CORE_RECORD_KIFUWRITER_HPP__

#include "core/record/Record.hpp"

namespace sunfish {

class KifuWriter {
public:

  KifuWriter() = delete;

  static bool write(std::ostream& os,
                    const Record& record,
                    const RecordInfo* info = nullptr);

};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_KIFUWRITER_HPP__
