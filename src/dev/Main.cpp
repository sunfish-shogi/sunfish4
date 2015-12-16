/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "core/util/CoreUtil.hpp"
#include "logger/Logger.hpp"

#include "dev/code_generator/ZobristCodeGenerator.hpp"

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // program options
  ProgramOptions po;
  po.addOption("gen-zobrist", "generate Zobrist.cpp");
  po.addOption("silent", "s", "silent mode");
  po.addOption("help", "h", "show this help");
  po.parse(argc, argv);

  // --help or -h
  if (po.has("help")) {
    std::cout << po.help();
    return 0;
  }

  // if '--silent' or '-s' is NOT specified.
  if (!po.has("silent")) {
    Loggers::message.addStream(std::cerr);
    Loggers::error.addStream(std::cerr, ESC_SEQ_COLOR_RED, ESC_SEQ_COLOR_RESET);
    Loggers::warning.addStream(std::cerr, ESC_SEQ_COLOR_YELLOW, ESC_SEQ_COLOR_RESET);
    Loggers::info.addStream(std::cerr, ESC_SEQ_COLOR_GREEN, ESC_SEQ_COLOR_RESET);
    Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
    Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
    Loggers::debug.addStream(std::cerr, ESC_SEQ_COLOR_CYAN, ESC_SEQ_COLOR_RESET);
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    OUT(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  // genearate src/core/position/Zobrist.cpp
  if (po.has("gen-zobrist")) {
    ZobristCodeGenerator generator;
    bool ok = generator.generate();

    return ok ? 0 : 1;
  }

  OUT(error) << "No action is specified.";

  return 1;
}
