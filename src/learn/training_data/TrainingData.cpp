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
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
    return false;
  }

  for (auto ite = trainingData_.begin(); ite != trainingData_.end(); ite++) {
    char sfenSize = static_cast<char>(ite->sfen.size());
    file.write(&sfenSize, sizeof(sfenSize));
    file.write(ite->sfen.c_str(), ite->sfen.size());
    uint16_t move = ite->move.serialize16();
    file.write(reinterpret_cast<char*>(&move), sizeof(move));
  }
  file.close();

  return true;
}

bool TrainingDataGenerator::loadCsaFiles(const char* dir) {
  Directory directory(dir);
  auto files = directory.files("*.csa");

  if (files.size() == 0) {
    LOG(error) << ".csa files not found";
    return false;
  }

  for (auto ite = files.begin(); ite != files.end(); ite++) {
    std::ifstream file(*ite);
    if (!file) {
      LOG(error) << "could not open a file: " << *ite;
      continue;
    }
    Record record;
    CsaReader::read(file, record);
    file.close();

    Position pos = record.initialPosition;
    for (const auto& move : record.moveList) {
      auto sfen = pos.toStringSFEN();
      trainingData_.push_back({ std::move(sfen), move });

      Piece captured;
      if (!pos.doMove(move, captured)) {
        LOG(error) << "an illegal move is detected: " << move.toString(pos) << "\n"
                   << pos.toString();
        break;
      }
    }
  }

  return true;
}

void TrainingDataGenerator::shuffle(Random& random) {
  random.shuffle(trainingData_.begin(), trainingData_.end());
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


bool TrainingDataReader::read(TrainingDataElement& elem) {
  char sfenSize;
  file_.read(&sfenSize, sizeof(sfenSize));
  if (file_.eof()) {
    return false;
  }

  char sfen[sfenSize+1];
  memset(sfen, 0, sizeof(sfen));
  file_.read(sfen, sfenSize);

  uint16_t move;
  file_.read(reinterpret_cast<char*>(&move), sizeof(move));

  elem.sfen = sfen;
  elem.move = Move::deserialize(move);

  return true;
}

} // namespace sunfish
