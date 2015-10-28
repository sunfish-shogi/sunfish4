/* main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "test/Test.hpp"
#include "logger/Logger.hpp"
#include <fstream>

#define DEFAULT_TEST_RESULT_FILENAME "test_result.xml"

using namespace sunfish;

int main(int argc, char** argv, char**) {
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
    Loggers::message.addStream(std::cerr, ESC_SEQ_COLOR_GREEN, ESC_SEQ_COLOR_RESET);
    Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
    Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
    Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
    Loggers::develop.addStream(std::cerr, ESC_SEQ_COLOR_WHITE, ESC_SEQ_COLOR_RESET);
  }

  // the name of the result file
  std::string resultFileName = DEFAULT_TEST_RESULT_FILENAME;
  if (po.has("out")) {
    resultFileName = po.getValue("out");
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    Loggers::warning << "WARNING: `" << invalidArgument.arg << "' is invalid argument: " << invalidArgument.reason;
  }

  // execute
  bool result = TestSuite::test();

  // write results to a file in xUnit format
  std::ofstream fout(resultFileName, std::ios::out);
  if (!fout) {
    Loggers::error << "open error: " << resultFileName;
    return 1;
  }
  fout << TestSuite::getXml();
  fout.close();

  // show result
  if (result) {
    Loggers::message << "Test passed.";
  } else {
    Loggers::error << "Test failed.";
  }
  Loggers::message << "See '" << resultFileName << "'.";

  // return value
  return result ? 0 : 1;
}
