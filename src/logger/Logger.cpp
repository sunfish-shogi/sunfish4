/* Logger.cpp
 * 
 * Kubo Ryosuke
 */

#include "logger/Logger.hpp"
#include "common/Def.hpp"
#include <chrono>
#include <ctime>
#include <cassert>

namespace sunfish {

const char* LoggerUtil::getIso8601() {
  using namespace std::chrono;
  THREAD_LOCAL char buf[22];
  std::time_t t = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
  std::tm m;
#ifdef WIN32
  gmtime_s(&m, &t);
#else
  gmtime_r(&t, &m);
#endif
  std::strftime(buf, sizeof(buf), "%FT%TZ ", &m);
  return buf;
}

std::mutex Logger::mutex_;
Logger Loggers::message;
Logger Loggers::error  ("[ERROR]");
Logger Loggers::warning("[WARN] ");
Logger Loggers::info   ("[INFO] ");
Logger Loggers::send   ("[SEND] ");
Logger Loggers::receive("[RECV] ");
#ifndef NDEBUG
Logger Loggers::debug  ("[DEBUG]");
#endif //NDEBUG

} // namespace sunfish
