/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/util/SearchUtil.hpp"
#include "expt/solve/Solver.hpp"
#include "expt/mgtest/MoveGenerationTest.hpp"
#include "logger/Logger.hpp"
#include <string>

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();
  SearchUtil::initialize();

  // program options
  ProgramOptions po;
  po.addOption("solve", "run a solver", true);
  po.addOption("mgtest", "run a cross-check test of move generation");
  po.addOption("time", "t", "a muximum time of search in seconds (This option is only used when the --solver option is specified.)", true);
  po.addOption("depth", "d", "a muximum depth of search (This option is only used when the --solver option is specified.)", true);
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
    OUT(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  // solver
  if (po.has("solve")) {
    Solver solver;

    auto config = solver.getConfig();
    if (po.has("time")) {
      config.muximumTimeSeconds = std::stoi(po.getValue("time"));
    }
    if (po.has("depth")) {
      config.muximumDepth = std::stoi(po.getValue("depth"));
    }
    solver.setConfig(config);

    std::string targetDirectory = po.getValue("solve");
    bool ok = solver.solve(targetDirectory);
    return ok ? 0 : 1;
  }

  // move generation test
  if (po.has("mgtest")) {
    MoveGenerationTest mgtest;
    bool ok = mgtest.test(500, 1000);
    return ok ? 0 : 1;
  }

  OUT(error) << "No action is specified.";
  std::cout << po.help();

  return 0;
}
