/* Test.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TEST_TEST_HPP__
#define SUNFISH_TEST_TEST_HPP__

#if !defined(NDEBUG)

#include "common/time/Timer.hpp"
#include "logger/Logger.hpp"
#include <typeinfo>
#include <sstream>
#include <string>
#include <map>
#include <utility>

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

using TEST_METHOD = void (*)();

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
  using TestCaseMethodMap = std::map<std::string, TEST_METHOD>;
  using TestSuiteMethodMap = std::map<std::string, TestCaseMethodMap>;

  TestSuiteMethodMap tests_;

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
   * Add the specified method for unit test into the suite.
   * This function is called by TEST macro.
   * You should not call this manually.
   */
  static void addTest(const char* groupName, const char* methodName, TEST_METHOD method) {
    auto ins = getInstance();
    ins->tests_[groupName][methodName] = method;
  }

  /**
   * Run all tests.
   */
  static bool test() {
    auto ins = getInstance();
    const auto& tests = ins->tests_;
    int totalErrors = 0;

    ins->results_.clear();

    for (auto it = tests.begin(); it != tests.end(); it++) {
      auto& groupName = it->first;
      auto& methods = it->second;
      int tests = 0;
      int errors = 0;
      int failures = 0;

      auto& tsr = ins->results_[groupName];

      for (auto im = methods.begin(); im != methods.end(); im++) {
        auto& methodName = im->first;
        auto method = im->second;

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
          OUT(error) << moss.str();

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

    oss << R"(<?xml version=\"1.0\"?>)" "\n";
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

    return oss.str();
  }

};

class test_inserter__ {
public:
  test_inserter__(const char* groupName, const char* methodName, TEST_METHOD method) {
    TestSuite::addTest(groupName, methodName, method);
  }
};

} // namespace sunfish

#endif // !defined(NDEBUG)

#endif // SUNFISH_TEST_TEST_HPP__
