/* CodeGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_DEV_CODEGENERATOR_CODEGENERATOR_HPP__
#define SUNFISH_DEV_CODEGENERATOR_CODEGENERATOR_HPP__

#include "logger/Logger.hpp"
#include <string>
#include <fstream>

namespace sunfish {

class CodeGenerator {
public:

  template <class T>
  CodeGenerator(T&& outputPath) : outputPath_(std::forward<T>(outputPath)) {
  }

  bool generate() {
    std::ofstream fout(outputPath_);
    if (!fout) {
      LOG(error) << "Could not open the output file: " << outputPath_;
      return false;
    }

    bool ok = generateIntoStream(fout);

    fout.close();

    return ok;
  }

protected:

  virtual bool generateIntoStream(std::ostream& os) = 0;

private:

  std::string outputPath_;

};

} // namespace sunfish

#endif // SUNFISH_DEV_CODEGENERATOR_CODEGENERATOR_HPP__
