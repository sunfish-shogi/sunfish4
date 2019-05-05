/* Csa2Kifu.cpp
 *
 * Kubo Ryosuke
 */

#include "tools/csa2kifu/Csa2Kifu.hpp"
#include "core/record/Record.hpp"
#include "core/record/CsaReader.hpp"
#include "core/record/KifuWriter.hpp"
#include <iostream>

namespace sunfish {

bool Csa2Kifu::run() {
  Record record;
  RecordInfo info;
  if (!CsaReader::read(std::cin, record, &info)) {
    LOG(error) << "an error is occured in CsaReader";
    return false;
  }

  return KifuWriter::write(std::cout, record, &info);
}

} // namespace sunfish
