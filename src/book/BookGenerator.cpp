/* BookGenerator.cpp
 *
 * Kubo Ryosuke
 */

#include "book/BookGenerator.hpp"
#include "common/file_system/Directory.hpp"
#include "common/file_system/FileUtil.hpp"
#include "core/record/CsaReader.hpp"
#include "core/util/PositionUtil.hpp"
#include "logger/Logger.hpp"
#include <fstream>

namespace sunfish {

bool BookGenerator::generate() {
  book_.clear();

  if (FileUtil::isDirectory(path_)) {
    // 'path_' points to a directory
    Directory directory(path_);
    auto files = directory.files("*.csa");
    for (const auto& path : files) {
      if (!generate(path)) {
        return false;
      }
    }
    return true;

  } else if (FileUtil::isFile(path_)) {
    // 'path_' points to a file
    return generate(path_);

  } else {
    // a specified path is not available.
    LOG(error) << "not available path: " << path_;
    return false;
  }

}

bool BookGenerator::generate(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
    OUT(info) << "";
    return false;
  }

  Record record;
  CsaReader::read(file, record);

  file.close();

  Position position = record.initialPosition;

  if (!handicap_ && !PositionUtil::isEvenInitialPosition(position)) {
    return true;
  }

  for (unsigned i = 0; i < record.moveList.size(); i++) {
    if (limit_ > 0 && i >= limit_) {
      break;
    }

    Move move = record.moveList[i];
    book_.insert(position, move);
    
    Piece captured;
    if (!position.doMove(move, captured)) {
      LOG(error) << "an illegal move is detected: " << move.toString(position) << "\n"
                 << position.toString();
      return false;
    }
  }

  return true;
}

} // namespace sunfish
