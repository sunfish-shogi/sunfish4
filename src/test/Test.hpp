/* Test.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TEST_TEST_HPP__
#define SUNFISH_TEST_TEST_HPP__

#include "common/time/Timer.hpp"
#include "logger/Logger.hpp"
#include <typeinfo>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <utility>

#define TEST_BEFORE(group) \
void test_method_before_ ## group();\
sunfish::test_before_inserter__ __test_before_inserter_ ## group (#group, test_method_before_ ## group); \
void test_method_before_ ## group()

#define TEST(group, name) \
void test_method_ ## group ## _ ## name();\
sunfish::test_inserter__ __test_inserter_ ## group ## _ ## name (#group, #name, test_method_ ## group ## _ ## name); \
void test_method_ ## group ## _ ## name()

#define ASSERT_EQ(correct, exact) do { \
  const auto correct_ = (correct); \
  const auto exact_ = (exact); \
  if (!((exact_) == (correct_))) {\
    std::ostringstream oss;\
    oss << "\"" << (exact_) << "\" is not equal to \"" << (correct_) << "\".";\
    throw sunfish::TestError{oss.str(), __FILE__, __LINE__};\
  } \
} while (false)

#define ASSERT_TRUE(b) do { \
  const auto b_ = (b);\
  if (!b_) { \
    throw sunfish::TestError{std::string(#b) + " is not true.", __FILE__, __LINE__}; \
  } \
} while (false)

#define ASSERT_FALSE(b) do { \
  const auto b_ = (b);\
  if (b_) { \
    throw sunfish::TestError{std::string(#b) + " is not false.", __FILE__, __LINE__}; \
  } \
} while (false)

namespace sunfish {

using TestMethod = void (*)();

/**
 * This error is thrown when the test is failed.
 */
struct TestError {
  std::string reason;
  const char* file;
  unsigned line;
};

class TestSuite {
public:
  struct Error {
    std::string message;
  };

  enum class State {
    PASSED,
    ERROR,
    FAILURE,
  };

  struct Result {
    State state;
    float time;
    Error error;
  };

  using TestCaseResultMap = std::map<std::string, Result>;

  struct TestSuiteResult {
    TestCaseResultMap results;
    int tests;
    int errors;
    int failures;
  };

  using TestSuiteResultMap = std::map<std::string, TestSuiteResult>;

private:
  struct TestMethodInfo {
    std::string name;
    TestMethod method;
  };
  using TestMethodMap = std::list<TestMethodInfo>;
  struct TestGroup {
    TestMethod before;
    TestMethodMap methods;
  };
  using TestGroupMap = std::map<std::string, TestGroup>;

  TestGroupMap groups_;

  TestSuiteResultMap results_;

  static TestSuite* getInstance() {
    static TestSuite instance;
    return &instance;
  }

  static std::string getErrorMessage(const TestError& e) {
    std::ostringstream oss;
    oss << e.file << ":" << e.line << ": " << e.reason;
    return oss.str();
  }

public:

  /**
   * Add the specified method which called before calling the test methods.
   * This function is called by TEST_BEFORE macro.
   * You should not call this manually.
   */
  static void addTestBefore(const char* groupName, TestMethod method) {
    auto ins = getInstance();
    ins->groups_[groupName].before = method;
  }

  /**
   * Add the specified method for unit test into the suite.
   * This function is called by TEST macro.
   * You should not call this manually.
   */
  static void addTest(const char* groupName, const char* methodName, TestMethod method) {
    auto ins = getInstance();
    ins->groups_[groupName].methods.push_back({
      methodName,
      method
    });
  }

  /**
   * Run all tests.
   */
  static bool test() {
    auto ins = getInstance();
    const auto& tests = ins->groups_;
    int totalErrors = 0;

    ins->results_.clear();

    for (auto it = tests.begin(); it != tests.end(); it++) {
      auto& groupName = it->first;
      auto& group = it->second;
      int tests = 0;
      int errors = 0;
      int failures = 0;

      auto& tsr = ins->results_[groupName];

      if (group.before) {
        group.before();
      }

      for (auto im = group.methods.begin(); im != group.methods.end(); im++) {
        auto& methodName = im->name;
        auto method = im->method;

        tests++;

        Timer timer;
        timer.start();

        try {
          // run
          method();

          // passed
          tsr.results[methodName].state = State::PASSED;
          tsr.results[methodName].time = timer.elapsed();

        } catch (const TestError& e) {
          auto message = getErrorMessage(e);

          // error
          errors++;
          tsr.results[methodName].state = State::ERROR;
          tsr.results[methodName].time = timer.elapsed();
          tsr.results[methodName].error.message = message;

          std::ostringstream moss;
          moss << '\n' << message;
          MSG(error) << moss.str();

        } catch (...) {
          // failure
          failures++;
          tsr.results[methodName].state = State::FAILURE;
          tsr.results[methodName].time = timer.elapsed();

        }
      }

      tsr.tests = tests;
      tsr.errors = errors;
      tsr.failures = failures;

      totalErrors += (errors + failures);
    }

    return totalErrors == 0;
  }

  /**
   * Get the results
   */
  static TestSuiteResultMap getResults() {
    auto ins = getInstance();
    return ins->results_;
  }

  /**
   * Get the results in XML format.
   */
  static std::string getXml() {
    auto ins = getInstance();
    std::ostringstream oss;

    oss << R"(<?xml version="1.0"?>)" "\n";
    oss << R"(<testsuites>)" << "\n";
    for (const auto& pair : ins->results_) {
      const auto& suiteName = pair.first;
      const auto& tsr = pair.second;

      oss << R"(<testsuite name=")" << suiteName << R"(" )"
             R"(tests=")" << tsr.tests << R"(" )"
             R"(errors=")" << tsr.errors << R"(" )"
             R"(failures=")" << tsr.failures << R"(" )"
             R"(skip="0">)" << "\n";

      for (const auto& pair : tsr.results) {
        const auto& testName = pair.first;
        const auto& result = pair.second;

        oss << "\t" R"(<testcase name=")" << testName << R"(" )"
                    R"(time=")" << result.time << R"(">)" << "\n";
        if (result.state == State::ERROR) {
          oss << "\t\t" R"(<error message=")" << result.error.message << R"(">)" << "\n";
          oss << "\t\t" R"(</error>)" << "\n";
        }
        oss << "\t" R"(</testcase>)" << "\n";
      }

      oss << R"(</testsuite>)" << "\n";
    }
    oss << R"(</testsuites>)" << "\n";

    return oss.str();
  }

};

class test_before_inserter__ {
public:
  test_before_inserter__(const char* groupName, TestMethod method) {
    TestSuite::addTestBefore(groupName, method);
  }
};

class test_inserter__ {
public:
  test_inserter__(const char* groupName, const char* methodName, TestMethod method) {
    TestSuite::addTest(groupName, methodName, method);
  }
};

} // namespace sunfish

#endif // SUNFISH_TEST_TEST_HPP__
