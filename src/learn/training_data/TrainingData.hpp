/* TrainingData.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__
#define SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__

#include "core/move/Move.hpp"
#include <vector>
#include <string>
#include <fstream>

namespace sunfish {

class Record;

class TrainingDataGenerator {
public:

  bool writeToFile(const char* path) const;

  bool writeToFile(const std::string& path) const {
    return writeToFile(path.c_str());
  }

  bool appendCsaFiles(const char* dir);

  bool appendCsaFiles(const std::string& dir) {
    return appendCsaFiles(dir.c_str());
  }

private:

  std::vector<std::string> csaFiles_;

};

class TrainingDataReader {
public:

  ~TrainingDataReader();

  bool open(const char* path);

  bool open(const std::string& path) {
    return open(path.c_str());
  }

  bool read(Record& record);

private:

  std::ifstream file_;

};

} // namespace sunfish

#endif // SUNFISH_LEARN_TRAININGDATA_TRAININGDATA_HPP__
