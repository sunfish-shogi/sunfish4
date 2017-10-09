/* ProgramOptions.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_PROGRAMOPTIONS_PROGRAMOPTIONS__
#define SUNFISH_COMMON_PROGRAMOPTIONS_PROGRAMOPTIONS__

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

namespace sunfish {

class ProgramOptions {
private:

  struct Option {
    std::string key;
    std::string shortKey;
    std::string description;
    bool arg;
    bool exists;
    std::string value;
  };

  struct InvalidArgument {
    std::string arg;
    std::string reason;
  };

  std::vector<Option> options_;
  std::vector<std::string> stdArguments_;
  std::vector<InvalidArgument> invalidArguments_;

  void setValue(const char* key, const char* value) {
    for (auto& option : options_) {
      if (option.key == key || option.shortKey == key) {
        option.exists = true;
        option.value = value;
        return;
      }
    }
  }

  const Option* getOption(const char* key, bool isShort) const {
    for (auto& option : options_) {
      if ((!isShort ? option.key : option.shortKey) == key) {
        return &option;
      }
    }
    return nullptr;
  }

public:

  ProgramOptions() = default;
  ProgramOptions(const ProgramOptions&) = delete;
  ProgramOptions(ProgramOptions&&) = delete;

  void addOption(const char* key, const char* description, bool arg = false) {
    options_.push_back(Option{ key, "", description, arg, false, "" });
  }

  void addOption(const char* key, const char* shortKey, const char* description, bool arg = false) {
    options_.push_back(Option{ key, shortKey, description, arg, false, "" });
  }

  void parse(int argc, char** argv) {
    const char* lastKeyFull = nullptr;
    const char* lastKey = nullptr;

    for (int i = 1; i <= argc; i++) {
      bool isLast = (i == argc);
      const char* arg = !isLast ? argv[i] : "";

      if (arg[0] == '-') {
        // option key
        bool isFullSpell = (arg[1] == '-');
        if (lastKey != nullptr) {
          invalidArguments_.push_back({ lastKeyFull, "this option requires a value" });
          lastKey = nullptr;
        }
        lastKey = &arg[isFullSpell?2:1];
        lastKeyFull = arg;
        auto opt = getOption(lastKey, !isFullSpell);
        if (opt == nullptr) {
          invalidArguments_.push_back({ lastKeyFull, "unknown option" });
          lastKey = nullptr;
        } else if (!opt->arg) {
          setValue(lastKey, "");
          lastKey = nullptr;
        }

      } else if (lastKey != nullptr) {
        // has last key
        if (!isLast) {
          setValue(lastKey, arg);
          lastKey = nullptr;
        } else {
          invalidArguments_.push_back({ lastKeyFull, "this option requires a value" });
          lastKey = nullptr;
        }

      } else if (!isLast) {
        // normal argument
        stdArguments_.push_back(arg);

      }
    }
  }

  bool has(const char* key) const {
    for (const auto& option : options_) {
      if (option.key == key || option.shortKey == key) {
        if (option.exists) {
        return true;
        }
      }
    }
    return false;
  }

  const char* getValue(const char* key) const {
    for (const auto& option : options_) {
      if (option.key == key || option.shortKey == key) {
        if (option.exists) {
        return option.value.c_str();
        }
      }
    }
    return "";
  }

  const std::vector<std::string>& getStdArguments() const {
    return stdArguments_;
  }

  const std::vector<InvalidArgument>& getInvalidArguments() const {
    return invalidArguments_;
  }

  std::string help() const {
    std::ostringstream oss;
    oss << "Option:\n";
    for (const auto& option : options_) {
      int length = 0;
      oss << "  ";
      if (!option.shortKey.empty()) {
        oss << '-' << option.shortKey << " [ --" << option.key << ']';
        length += (int)(option.shortKey.length() + option.key.length() + 7);
      } else {
        oss << "--" << option.key;
        length += (int)(option.key.length() + 2);
      }
      if (option.arg) {
        oss << " arg";
        length += 4;
      }
      int indent = std::max(1, (int)(24 - length));
      for (int i = 0; i < indent; i++) {
        oss << ' ';
      }
      oss << option.description << '\n';
    }
    return oss.str();
  }
};

} // namespace sunfish

#endif // SUNFISH_COMMON_PROGRAMOPTIONS_PROGRAMOPTIONS__
