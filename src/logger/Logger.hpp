/* Logger.hpp
 * 
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LOGGER_LOGGER_HPP__
#define SUNFISH_LOGGER_LOGGER_HPP__

#include <iostream>
#include <vector>
#include <ctime>
#include <mutex>
#include <memory>
#include <utility>

#define __FILE_LINE__ (__FILE__ ":" __L2STR(__LINE__))
#define __L2STR(l) L2STR__(l)
#define L2STR__(l) #l

namespace sunfish {

class LoggerUtil {
private:

  LoggerUtil();

public:

  static const char* getIso8601();

};

class Logger {
private:

  struct Stream {
    std::ostream* pout;
    bool timestamp;
    bool loggerName;
    const char* before;
    const char* after;
  };

  enum class LineState {
    Top,
    Normal,
    End,
    All,
  };

public:

  class SubLogger {
  private:
    struct Data {
      Logger* plogger;
      std::lock_guard<std::mutex> lock;
      Data(Logger* plogger, std::mutex& mutex) : plogger(plogger), lock(mutex) {
      }
      ~Data() {
        plogger->printNoLock<LineState::End>("");
      }
    };
    std::shared_ptr<Data> data;

  public:
    SubLogger(const SubLogger& org) = default;
    SubLogger(Logger* plogger, std::mutex& mutex) {
      data = std::make_shared<Data>(plogger, mutex);
    }
    template <class T>
    SubLogger& operator<<(T&& t) {
      data->plogger->printNoLock<LineState::Normal>(std::forward<T>(t));
      return *this;
    }
  };

  Logger(const char* name = nullptr) : name_(name) {
  }
  Logger(const Logger& logger) = delete;
  Logger(Logger&& logger) = delete;

  void addStream(std::ostream& o, bool timestamp, bool loggerName,
      const char* before, const char* after) {
    Stream s = { &o, timestamp, loggerName, before, after };
    os_.push_back(s);
  }
  void addStream(std::ostream& o, bool timestamp, bool loggerName) {
    addStream(o, timestamp, loggerName, nullptr, nullptr);
  }
  void addStream(std::ostream& o, const char* before, const char* after) {
    addStream(o, false, false, before, after);
  }
  void addStream(std::ostream& o) {
    addStream(o, false, false, nullptr, nullptr);
  }

  template <class T> void print(T&& t) {
    std::lock_guard<std::mutex> lock(mutex_);
    printNoLock<LineState::All>(std::forward<T>(t));
  }

  template <class T>
  SubLogger operator<<(T&& t) {
    SubLogger s(this, mutex_);
    printNoLock<LineState::Top>(std::forward<T>(t));
    return s;
  }

private:

  template <LineState ls, class T>
  void printNoLock(T&& t) {
    std::vector<Stream>::iterator it;
    for (it = os_.begin(); it != os_.end(); it++) {
      if (ls == LineState::Top || ls == LineState::All) {
        // prefix
        if (it->before != nullptr) {
          *(it->pout) << it->before;
        }

        // timestamp
        if (it->timestamp) {
          *(it->pout) << LoggerUtil::getIso8601();
        }

        // logger name
        if (it->loggerName && name_) {
          *(it->pout) << '[' << name_ << "] ";
        }
      }

      // main data
      *(it->pout) << std::forward<T>(t);

      if (ls == LineState::End || ls == LineState::All) {
        // suffix
        if (it->after != nullptr) {
          *(it->pout) << it->after;
        }

        // flush
        *(it->pout) << '\n';
        it->pout->flush();
      }
    }
  }

  static std::mutex mutex_;
  const char* name_;
  std::vector<Stream> os_;

};

class Loggers {
public:
  static Logger error;
  static Logger warning;
  static Logger message;
  static Logger send;
  static Logger receive;
#ifndef NDEBUG
  static Logger debug;
  static Logger test;
  static Logger develop;
#endif //NDEBUG
};

} // namespace sunfish

#endif // SUNFISH_LOGGER_LOGGER_HPP__
