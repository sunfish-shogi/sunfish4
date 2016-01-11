/* Sfen2Csa.cpp
 *
 * Kubo Ryosuke
 */

#include "tools/sfen2csa/Sfen2Csa.hpp"
#include "common/string/StringUtil.hpp"
#include "core/record/Record.hpp"
#include "core/record/SfenParser.hpp"
#include "core/record/CsaWriter.hpp"
#include <iostream>

namespace sunfish {

bool Sfen2Csa::run() {
  while (true) {
    std::string line;
    std::getline(std::cin, line);

    if (std::cin.eof()) {
      return true;
    }

    auto args = StringUtil::split(line, [](char c) {
      return isspace(c);
    });

    Record record;
    bool ok = SfenParser::parseUsiCommand(args.begin(),
                                          args.end(),
                                          record);

    if (!ok) {
      LOG(error) << "an error is occured in SfenParser";
      return false;
    }

    CsaWriter::write(std::cout, record);
  }
}

} // namespace sunfish
