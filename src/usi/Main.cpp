/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "core/util/CoreUtil.hpp"
#include "usi/client/UsiClient.hpp"
#include "logger/Logger.hpp"
#include <iostream>
#include <fstream>

namespace {

const char* DefaultUsiLogFilename = "usi.log";
bool EnableLogFile = true;

} // namespace

using namespace sunfish;

int main(int, char**, char**) {
  // initialize static objects
  CoreUtil::initialize();

  // Logger settings
  std::ofstream fout;
  if (EnableLogFile) {
    fout.open(DefaultUsiLogFilename, std::ios::out | std::ios::app);

    Loggers::error.addStream(fout, true, true);
    Loggers::warning.addStream(fout, true, true);
    Loggers::info.addStream(fout, true, true);
    Loggers::send.addStream(fout, true, true);
    Loggers::receive.addStream(fout, true, true);
  }

  // USI client
  UsiClient client;

  bool ok = client.start();

  return ok ? 0 : 1;
}
