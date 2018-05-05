/* TrainingData.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/training_data/TrainingData.hpp"
#include "core/record/Record.hpp"
#include "core/record/CsaReader.hpp"
#include "common/file_system/Directory.hpp"
#include <cstring>

namespace sunfish {

bool TrainingDataGenerator::writeToFile(const char* path) const {
  std::ofstream datFile(path, std::ios::out | std::ios::binary);
  if (!datFile) {
    LOG(error) << "could not open a file: " << path;
    return false;
  }

  int skipped = 0;
  int error = 0;
  for (auto ite = csaFiles_.begin(); ite != csaFiles_.end(); ite++) {
    std::ifstream csaFile(*ite);
    if (!csaFile) {
      LOG(error) << "could not open a file: " << *ite;
      error++;
      continue;
    }
    Record record;
    CsaReader::read(csaFile, record);
    csaFile.close();

    /*
    if (record.specialMove != "%TORYO") {
      skipped++;
      continue;
    }
    */

    if (!record.initialPosition.isInitial(Position::Handicap::Even)) {
      skipped++;
      continue;
    }

    Position pos = record.initialPosition;
    for (const auto& move : record.moveList) {
      Piece captured;
      if (!pos.doMove(move, captured)) {
        LOG(error) << "an illegal move is detected: " << move.toString(pos) << "\n"
                   << pos.toString();
        break;
      }

      // write move
      uint16_t m16 = move.serialize16();
      datFile.write(reinterpret_cast<char*>(&m16), sizeof(m16));
    }

    // write end-of-moves marker
    uint16_t n16 = Move::none().serialize16();
    datFile.write(reinterpret_cast<char*>(&n16), sizeof(n16));
  }
  datFile.close();

  if (error != 0) {
    LOG(warning) << "errors are occured from " << error << " files";
  }

  if (skipped != 0) {
    LOG(warning) << skipped << " files are skipped";
  }

  return true;
}

bool TrainingDataGenerator::appendCsaFiles(const char* dir) {
  Directory directory(dir);
  auto files = directory.files("*.csa");

  if (files.size() == 0) {
    LOG(error) << ".csa files not found";
    return false;
  }

  for (auto ite = files.begin(); ite != files.end(); ite++) {
    csaFiles_.push_back(*ite);
  }
  return true;
}

TrainingDataReader::~TrainingDataReader() {
  if (file_.is_open()) {
    file_.close();
  }
}

bool TrainingDataReader::open(const char* path) {
  file_.open(path, std::ios::in | std::ios::binary);
  return !file_.fail();
}


bool TrainingDataReader::read(Record& record) {
  record.initialPosition.initialize(Position::Handicap::Even);
  record.moveList.clear();
  for (;;) {
    // read move
    uint16_t m16;
    file_.read(reinterpret_cast<char*>(&m16), sizeof(m16));
    if (file_.eof()) {
      return false;
    } else if (!file_) {
      LOG(error) << "failed to read training data file";
      return false;
    }
    Move move = Move::deserialize(m16);
    if (move.isNone()) {
      break;
    }
    record.moveList.push_back(move);
  }

  return true;
}

} // namespace sunfish
