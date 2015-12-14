/* Benchmark.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFIS_BENCHMARK_BENCHMARK_HPP__
#define SUNFIS_BENCHMARK_BENCHMARK_HPP__

#include "common/Def.hpp"
#include "common/time/Timer.hpp"
#include "logger/Logger.hpp"

#include <cassert>
#include <cstdint>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <type_traits>
#include <utility>

#if WIN32
# include <windows.h>
#else
# include <csignal>
# include <unistd.h>
#endif

#define BENCHMARK(n, f) auto n ## __bm_object__ = sunfish::create_bm_object__(#n, (f))

namespace sunfish {

extern void* enabler;

using bmstr_t = const char*;

template <class T>
inline const char* BMSTR(T&& str) {
  return static_cast<const char*>(str);
}

using BMTimeType = uint64_t;
using BMCounterType = uint64_t;

class BenchmarkController {
public:

  BenchmarkController(BMTimeType usec) : usec_(usec) {
  }

  void start() {
    interrupted_ = false;
    count_ = 0;
    swap(this);
    timer_.start();
#ifdef WIN32
    timeSetEvent(usec_ / 1000, 10, sigtime, 0, TIME_ONESHOT);
#else
    signal(SIGALRM, sigalrm);
    if (usec_ < 1000 * 1000) {
      ualarm(usec_, 0);
    } else {
      alarm(usec_ / (1000 * 1000));
    }
#endif
  }

  bool cont() {
    count_++;
    if (interrupted_) {
      elapsed_ = timer_.getElapsed() * 1000000;
      return false;
    }
    return true;
  }

  BMCounterType getCount() const {
    return count_;
  }

  BMTimeType getElapsed() const {
    return elapsed_;
  }

private:

  static BenchmarkController* swap(BenchmarkController* newIns) {
    static BenchmarkController* ins;
    auto prevIns = ins;
    ins = newIns;
    return prevIns;
  }

#ifdef WIN32
  static VOID CALLBACK sigalrm(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
#else
  static void sigalrm(int)
#endif
  {
    auto ins = swap(nullptr);
    assert(ins != nullptr);
    assert(!ins->interrupted_);
    ins->interrupted_ = true;
  }

  volatile bool interrupted_;
  BMTimeType usec_;
  BMCounterType count_;
  Timer timer_;
  BMTimeType elapsed_;

};

class BenchmarkSuite {
public:

  struct Entry {
    std::string params;
    std::function<void(BenchmarkController&)> function;
    BMTimeType time;
  };

  template <class T>
  static void addInitializer(intptr_t ptr, T&& initializer) {
    auto& ins = getInstance();
    ins.initializerMap_[ptr] = std::forward<T>(initializer);
  }

  static void removeInitializer(intptr_t ptr) {
    auto& ins = getInstance();
    ins.initializerMap_.erase(ptr);
  }

  template <class T, class U, class V>
  static void addTest(T&& name, U&& params, BMTimeType time, V&& function) {
    auto& ins = getInstance();
    ins.entries_[name].push_back({
        std::forward<U>(params),
        std::forward<V>(function),
        time,
        });
  }

  static void initialize() {
    auto& ins = getInstance();
    for (const auto& pair : ins.initializerMap_) {
      const auto& func = pair.second;
      func();
    }
  }

  static void run() {
    auto& ins = getInstance();
    for (const auto& pair : ins.entries_) {
      const auto& name = pair.first;
      const auto& list = pair.second;
      run(name, list);
    }
  }

private:

  static void run(const std::string& name, const std::vector<Entry>& list) {
    OUT(message) << "Benchmark                                        Time[us]      Iterations";
    OUT(message) << "-------------------------------------------------------------------------";

    for (const auto& entry : list) {
      BenchmarkController bc(entry.time);

      entry.function(bc);

      std::ostringstream longName;
      longName << name;
      if (!entry.params.empty()) {
        longName << '/' << entry.params;
      }

      float ips = (float)bc.getCount() / bc.getElapsed() * 1e+6;

      OUT(message)
        << std::setw(41) << std::left  << longName.str() << ' '
        << std::setw(15) << std::right << bc.getElapsed() << ' '
        << std::setw(15) << std::right << bc.getCount() << ' '
        << ips << "/sec";
    }

    OUT(message) << "";
  }

  BenchmarkSuite() = default;
  BenchmarkSuite(const BenchmarkSuite&) = delete;
  BenchmarkSuite(BenchmarkSuite&&) = delete;

  static BenchmarkSuite& getInstance() {
    static BenchmarkSuite ins;
    return ins;
  }

  std::map<std::string, std::vector<Entry>> entries_;
  std::map<intptr_t, std::function<void()>> initializerMap_;

};

template <class Func>
class bm_object__ {
public:

  using FuncType = Func;
  using WrappedFuncType = std::function<void(BenchmarkController&)>;

  struct FunctionWrapper {
    std::shared_ptr<WrappedFuncType> function;
    std::string params;
  };

  static CONSTEXPR_CONST BMTimeType DefaultMicroSecondTime = 32 * 1000;

  template <class T>
  bm_object__(T&& name, FuncType&& function)
  : name_(std::forward<T>(name)),
    time_(DefaultMicroSecondTime),
    function_(std::make_shared<FuncType>(std::forward<FuncType>(function))) {
    setInitializer();
  }

  bm_object__* time(BMTimeType time) {
    time_ = time;
    return this;
  }

  template <class... Args>
  bm_object__* args(Args&&... args) {
    std::string pmstr = visualizeParams(args...);

    auto func = std::bind([](const std::shared_ptr<FuncType>& func, BenchmarkController& bc, const Args&... args) {
      (*func)(bc, args...);
    }, function_, std::placeholders::_1, std::forward<Args>(args)...);

    functionWrappers_.push_back({ std::make_shared<WrappedFuncType>(func), pmstr });

    return this;
  }

  const std::string getName() const {
    return name_;
  }

  BMTimeType getTime() const {
    return time_;
  }

  const FuncType& getFunction() const {
    return *function_.get();
  }

  const std::vector<FunctionWrapper> getFunctionWrappers() const {
    return functionWrappers_;
  }

private:

  void setInitializer() const {
    intptr_t ptr = reinterpret_cast<intptr_t>(this);
    BenchmarkSuite::addInitializer(ptr, [this]() {
      register_();
    });
  }

  void register_() const;

  template <class... Args>
  static std::string visualizeParams(const Args&... args) {
    std::ostringstream oss;
    p2s(oss, args...);

    std::string str = oss.str();

    while (true) {
      auto pos = str.find('\n');
      if (pos == std::string::npos) {
        break;
      }
      str.replace(pos, pos + 1, "");
    }

    if (str.length() > 15) {
      str = str.substr(0, 12);
      str += "...";
    }

    return str;
  }

  template <class T, class... Args>
  static void p2s(std::ostream& oss, const T& param, const Args&... args) {
    oss << param << '/';
    p2s(oss, args...);
  }

  template <class T>
  static void p2s(std::ostream& oss, const T& param) {
    oss << param;
  }

  std::string name_;
  BMTimeType time_;
  std::shared_ptr<FuncType> function_;
  std::vector<FunctionWrapper> functionWrappers_;

};

template <class F, class... Args>
struct is_callable {
  template <class T>
  static std::true_type check(decltype(std::declval<T>()(std::declval<Args>()...)) *);
  template <class T>
  static std::false_type check(...);
  typedef decltype(check<F>(nullptr)) type;
  static CONSTEXPR_CONST bool value = type::value;
};

template <class T, typename std::enable_if<is_callable<typename T::FuncType, BenchmarkController&>::value>::type*& = enabler>
inline void register_bm_object(const T* p) {
  const auto& name = p->getName();
  BMTimeType time = p->getTime();
  BenchmarkSuite::addTest(name, "", time, [p](BenchmarkController& bc) {
    p->getFunction()(bc);
  });
}

template <class T, typename std::enable_if<!is_callable<typename T::FuncType, BenchmarkController&>::value>::type*& = enabler>
inline void register_bm_object(const T* p) {
  const auto& name = p->getName();
  BMTimeType time = p->getTime();
  const auto& functionWrappers = p->getFunctionWrappers();

  if (functionWrappers.empty()) {
    LOG(error) << "ERROR: The benchmark test named " << name << " has no argument list.";
    exit(2);
  }

  for (const auto& fw : functionWrappers) {
    const auto& params = fw.params;
    const auto& fp = fw.function;
    BenchmarkSuite::addTest(name, params, time, [fp](BenchmarkController& bc) {
      (*fp.get())(bc);
    });
  }
}

template <class T>
inline void bm_object__<T>::register_() const {
  register_bm_object(this);
}

template <class T, class Func>
inline bm_object__<Func>* create_bm_object__(T&& name, Func&& function) {
  return new bm_object__<Func>(std::forward<T>(name), std::forward<Func>(function));
}

}

#endif // SUNFIS_BENCHMARK_BENCHMARK_HPP__
