/* ScopedThread.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_THREAD_SCOPEDTHREAD_HPP__
#define SUNFISH_COMMON_THREAD_SCOPEDTHREAD_HPP__

#include <thread>

namespace sunfish {

class ScopedThread {
public:

  ScopedThread() {
  }

  ~ScopedThread() {
    if (stop_) {
      stop_();
    }
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  template <class T, class U>
  void start(T&& proc,
             U&& stop) {
    thread_ = std::thread(proc);
    stop_ = std::forward<U>(stop);
  }

private:

  std::thread thread_;
  std::function<void()> stop_;

};

} // namespace sunfish

#endif // SUNFISH_COMMON_THREAD_SCOPEDTHREAD_HPP__
