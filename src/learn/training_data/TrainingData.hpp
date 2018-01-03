/* TrainingData.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__
#define SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__

#include "core/move/Move.hpp"
#include "common/math/Random.hpp"
#include <vector>
#include <string>
#include <fstream>

namespace sunfish {

struct TrainingDataElement {
  std::string sfen;
  sunfish::Move move;
};

class TrainingDataGenerator {
public:

  bool writeToFile(const char* path) const;

  bool writeToFile(const std::string& path) const {
    return writeToFile(path.c_str());
  }

  bool loadCsaFiles(const char* dir);

  bool loadCsaFiles(const std::string& dir) {
    return loadCsaFiles(dir.c_str());
  }

  void shuffle(Random&);

  size_t size() const {
    return trainingData_.size();
  }

private:

  std::vector<TrainingDataElement> trainingData_;

};

class TrainingDataReader {
public:

  ~TrainingDataReader();

  bool open(const char* path);

  bool open(const std::string& path) {
    return open(path.c_str());
  }

  bool read(TrainingDataElement& elem);

private:

  std::ifstream file_;

};

} // namespace sunfish

#endif // SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__
