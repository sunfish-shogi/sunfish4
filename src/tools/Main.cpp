/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "core/util/CoreUtil.hpp"
#include "book/Book.hpp"
#include "book/BookGenerator.hpp"
#include "logger/Logger.hpp"
#include "tools/sfen2csa/Sfen2Csa.hpp"

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();

  // program options
  ProgramOptions po;
  po.addOption("sfen2csa", "SFEN-CSA converter");
  po.addOption("gen-book", "generate opening book", true);
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
#ifndef NDEBUG
    Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
#endif // NDEBUG
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  // sfen2csa
  if (po.has("sfen2csa")) {
    Sfen2Csa s2c;
    bool ok = s2c.run();
    return ok ? 0 : 1;
  }

  // gen-book
  if (po.has("gen-book")) {
    auto path = po.getValue("gen-book");
    BookGenerator bg(path);
    bg.setLimit(20);
    if (!bg.generate()) {
      return 1;
    }
    bg.getBook().save();
    return 0;
  }

  MSG(error) << "No action is specified.";
  std::cout << po.help();

  return 0;
}
