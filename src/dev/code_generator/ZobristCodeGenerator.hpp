/* ZobristCodeGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_DEV_CODEGENERATOR_ZOBRISTCODEGENERATOR_HPP__
#define SUNFISH_DEV_CODEGENERATOR_ZOBRISTCODEGENERATOR_HPP__

#include "dev/code_generator/CodeGenerator.hpp"
#include <fstream>
#include <utility>

namespace sunfish {

class ZobristCodeGenerator : public CodeGenerator {
public:

  ZobristCodeGenerator();

protected:

  virtual bool generateIntoStream(std::ostream& os) override final;

};

} // namespace sunfish

#endif // SUNFISH_DEV_CODEGENERATOR_ZOBRISTCODEGENERATOR_HPP__
