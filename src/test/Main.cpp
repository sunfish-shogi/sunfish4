/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/util/SearchUtil.hpp"
#include "test/Test.hpp"
#include "logger/Logger.hpp"
#include <fstream>

#define DEFAULT_TEST_RESULT_FILENAME "test_result.xml"

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();
  SearchUtil::initialize();

  // program options
  ProgramOptions po;
  po.addOption("silent", "s", "silent mode");
  po.addOption("out", "o", "output file name (default: " DEFAULT_TEST_RESULT_FILENAME ")", true);
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
    Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
  }

  // the name of the result file
  std::string resultFileName = DEFAULT_TEST_RESULT_FILENAME;
  if (po.has("out")) {
    resultFileName = po.getValue("out");
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  // execute
  bool result = TestSuite::test();

  // write results to a file in xUnit format
  std::ofstream fout(resultFileName, std::ios::out);
  if (!fout) {
    MSG(error) << "Could not open output file: " << resultFileName;
    return 1;
  }
  fout << TestSuite::getXml();
  fout.close();

  // show result
  if (result) {
    MSG(info) << ESC_SEQ_COLOR_GREEN << "Test passed." << ESC_SEQ_COLOR_RESET;
  } else {
    MSG(error) << "Test failed.";
  }
  MSG(info) << "See '" << resultFileName << "'.";

  // return value
  return result ? 0 : 1;
}
