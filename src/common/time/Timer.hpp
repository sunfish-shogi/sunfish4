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

  /**
   * Set current time to a reference point.
   */
  void start() {
    base_ = std::chrono::system_clock::now();
  }

  /**
   * Get a real value representing a elapsed time from the reference point.
   */
  float elapsed(bool roundup = false) const {
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - base_;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    auto count = milliseconds.count();
    if (roundup) {
      count += static_cast<std::chrono::milliseconds::rep>(1);
    }
    return count * 1.0e-3f;
  }

  /**
   * Get an integer value representing a elapsed time from the reference point.
   */
  uint32_t elapsedInt(bool roundup = false) const {
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - base_;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    auto count = milliseconds.count();
    if (roundup) {
      count += static_cast<std::chrono::milliseconds::rep>(500);
    }
    return count / 1000;
  }

private:

  std::chrono::time_point<std::chrono::system_clock> base_;

};

} // namespace sunfish

#endif // SUNFISH_COMMON_TIME_TIMER_HPP__
