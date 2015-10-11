/* main.cpp
 *
 * Kubo Ryosuke
 */

#include "cui/Def.hpp"
#include "test/Test.hpp"
#include "logger/Logger.hpp"
#include <fstream>

#define TEST_OUT_FILENAME "test_result.xml"

using namespace sunfish;

int main(int, char**, char**) {
  // logger settings
  Loggers::error.addStream(std::cerr, ESC_SEQ_COLOR_RED, ESC_SEQ_COLOR_RESET);
  Loggers::warning.addStream(std::cerr, ESC_SEQ_COLOR_YELLOW, ESC_SEQ_COLOR_RESET);
  Loggers::message.addStream(std::cerr, ESC_SEQ_COLOR_GREEN, ESC_SEQ_COLOR_RESET);
  Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
  Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
  Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
  Loggers::develop.addStream(std::cerr, ESC_SEQ_COLOR_WHITE, ESC_SEQ_COLOR_RESET);

  // execute
  bool result = TestSuite::test();

  std::ofstream fout(TEST_OUT_FILENAME, std::ios::out);
  if (!fout) {
    Loggers::error << "open error: " << TEST_OUT_FILENAME;
    return 1;
  }
  fout << TestSuite::getXml();
  fout.close();

  if (result) {
    Loggers::message << "Test passed.";
  } else {
    Loggers::error << "Test failed.";
  }
  Loggers::message << "See '" << TEST_OUT_FILENAME << "'.";

  // return value
  return result ? 0 : 1;
}
