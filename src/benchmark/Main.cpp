/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "core/util/CoreUtil.hpp"
#include "benchmark/Benchmark.hpp"
#include "logger/Logger.hpp"
#include <fstream>

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();

  // program options
  ProgramOptions po;
  po.addOption("silent", "s", "silent mode");
  po.addOption("out", "o", "output file name", true);
  po.addOption("help", "h", "show this help");
  po.parse(argc, argv);

  // --help or -h
  if (po.has("help")) {
    std::cout << po.help();
    return 0;
  }

  // if '--silent' or '-s' is NOT specified.
  if (!po.has("silent")) {
    Loggers::error.addStream(std::cerr, ESC_SEQ_COLOR_RED, ESC_SEQ_COLOR_RESET);
    Loggers::warning.addStream(std::cerr, ESC_SEQ_COLOR_YELLOW, ESC_SEQ_COLOR_RESET);
    Loggers::info.addStream(std::cerr);
    Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
    Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
#if !defined(NDEBUG)
    Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
#endif // !defined(NDEBUG)
  }

  // the name of the result file
  std::ofstream fout;
  if (po.has("out")) {
    std::string resultFileName = po.getValue("out");

    fout.open(resultFileName, std::ios::out);
    if (!fout) {
      MSG(error) << "Could not open output file: " << resultFileName;
      return 1;
    }

    Loggers::info.addStream(fout);
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  // initialization
  BenchmarkSuite::initialize();

  // execute
  BenchmarkSuite::run();

  return 0;
}
