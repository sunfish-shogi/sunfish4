/* Timer.h
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_TIME_TIMER_HPP__
#define SUNFISH_COMMON_TIME_TIMER_HPP__

#include <chrono>

namespace sunfish {

class Timer {
public:

  Timer(bool roundup = false) : roundup_(roundup) {
  }

  /**
   * set current time to base
   */
  void set() {
    base_ = std::chrono::system_clock::now();
  }

  /**
   * get current time(sec) from base
   */
  float get() const {
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - base_;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    auto count = milliseconds.count();
    if (roundup_) {
      count += static_cast<std::chrono::milliseconds::rep>(1);
    }
    return count * 1.0e-3f;
  }

private:

  std::chrono::time_point<std::chrono::system_clock> base_;
  bool roundup_;

};

} // namespace sunfish

#endif // SUNFISH_COMMON_TIME_TIMER_HPP__
