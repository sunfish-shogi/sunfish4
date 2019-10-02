/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/resource/Resource.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/util/SearchUtil.hpp"
#include "usi/client/UsiClient.hpp"
#include "logger/Logger.hpp"
#include <iostream>
#include <fstream>

namespace {
namespace resources {

const char* UsiLogPath = "res/strings/usi_log_path";

} // namespace resources
} // namespace

using namespace sunfish;

int main(int, char**, char**) {
  // initialize static objects
  CoreUtil::initialize();
  SearchUtil::initialize();

  // Logger settings
  std::ofstream fout;
  auto logPath = Resource::string(resources::UsiLogPath, "");
  if (!logPath.empty()) {
    fout.open(logPath, std::ios::out | std::ios::app);

    Loggers::error.addStream(fout, true, true);
    Loggers::warning.addStream(fout, true, true);
    Loggers::info.addStream(fout, true, true);
    Loggers::send.addStream(fout, true, true);
    Loggers::receive.addStream(fout, true, true);
  }

  // USI client
  UsiClient().start();

  return 0;
}
