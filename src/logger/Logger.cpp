/* Logger.cpp
 * 
 * Kubo Ryosuke
 */

#include "logger/Logger.hpp"
#include "common/Def.hpp"
#include <chrono>
#include <ctime>

namespace sunfish {

std::string LoggerUtil::getIso8601() {
  using namespace std::chrono;
  std::time_t t = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
  std::tm m;
#ifdef WIN32
  gmtime_s(&m, &t);
#else
  gmtime_r(&t, &m);
#endif
  std::string buf(22, '\0');
  std::strftime(&buf[0], 22, "%FT%TZ ", &m);
  return buf;
}

std::mutex Logger::mutex_;
Logger Loggers::error  ("[ERROR]");
Logger Loggers::warning("[WARN] ");
Logger Loggers::info   ("[INFO] ");
Logger Loggers::send   ("[SEND] ");
Logger Loggers::receive("[RECV] ");
#ifndef NDEBUG
Logger Loggers::debug  ("[DEBUG]");
#endif //NDEBUG

} // namespace sunfish
