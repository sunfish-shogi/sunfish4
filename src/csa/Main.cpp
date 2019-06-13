/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "common/resource/Resource.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/util/SearchUtil.hpp"
#include "csa/client/CsaClient.hpp"
#include "logger/Logger.hpp"
#include <iostream>
#include <fstream>

namespace {
namespace resources {

const char* CsaLogPath = "res/strings/csa_log_path";

} // namespace resources
} // namespace

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();
  SearchUtil::initialize();

  // program options
  ProgramOptions po;
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
    Loggers::error.addStream(std::cerr, ESC_SEQ_COLOR_RED, ESC_SEQ_COLOR_RESET);
    Loggers::warning.addStream(std::cerr, ESC_SEQ_COLOR_YELLOW, ESC_SEQ_COLOR_RESET);
    Loggers::info.addStream(std::cerr);
    Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
    Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
  }

  // log file
  std::ofstream fout;
  auto logPath = Resource::string(resources::CsaLogPath, "");
  if (!logPath.empty()) {
    fout.open(logPath, std::ios::out | std::ios::app);

    Loggers::error.addStream(fout, true, true);
    Loggers::warning.addStream(fout, true, true);
    Loggers::info.addStream(fout, true, true);
    Loggers::send.addStream(fout, true, true);
    Loggers::receive.addStream(fout, true, true);
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(error) << "ERROR: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }
  if (!po.getInvalidArguments().empty()) {
    return 1;
  }

  // CSA client
  CsaClient client;

  bool ok = client.start();

  return ok ? 0 : 1;
}
