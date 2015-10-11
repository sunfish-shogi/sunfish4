/* Test.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_TEST_TEST_HPP__
#define SUNFISH_TEST_TEST_HPP__

#if !defined(NDEBUG)

#include "logger/Logger.hpp"
#include <typeinfo>
#include <sstream>
#include <string>
#include <map>

#define TEST(group, name) \
void test_method_ ## group ## name();\
sunfish::test_adder__ __test_adder_ ## group ## name (#group, #name, test_method_ ## group ## name); \
void test_method_ ## group ## name()

#define TEST_RESULT__(reason) throw sunfish::TestError{(reason), __FILE__, __LINE__}

#define ASSERT_EQ(correct, exact) do { \
  const auto correct_ = (correct); \
  const auto exact_ = (exact); \
  if (!((exact_) == (correct_))) {\
    std::ostringstream oss;\
    oss << "\"" << (exact_) << "\" is not equal to \"" << (correct_) << "\".";\
    TEST_RESULT__(oss.str());\
  } \
} while (false)

#define ASSERT(b) do { \
  const auto b_ = (b);\
  if (!b_) { TEST_RESULT__(std::string(#b) + " is not true."); } \
} while (false)

namespace sunfish {

using TEST_METHOD = void (*)();

/**
 * error
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

  struct Result {
    bool passed;
    Error error;
  };

  using TestCaseResultMap = std::map<std::string, Result>;

  struct TestSuiteResult {
    TestCaseResultMap results;
    int tests;
    int errors;
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
    oss << e.reason << " (" << e.file << ":" << e.line << ")";
    return oss.str();
  }

public:

  static void addTest(const char* groupName, const char* methodName, TEST_METHOD method) {
    auto ins = getInstance();
    ins->tests_[groupName][methodName] = method;
  }

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

      auto& tsr = ins->results_[groupName];

      for (auto im = methods.begin(); im != methods.end(); im++) {
        auto& methodName = im->first;
        auto method = im->second;

        tests++;

        try {
          method();
          tsr.results[methodName].passed = true;
        } catch (TestError e) {
          errors++;
          tsr.results[methodName].passed = false;
          tsr.results[methodName].error.message = getErrorMessage(e);
        }
      }

      tsr.tests = tests;
      tsr.errors = errors;

      totalErrors += errors;
    }

    return totalErrors == 0;
  }

  static TestSuiteResultMap getResults() {
    auto ins = getInstance();
    return ins->results_;
  }

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
             R"(failures="0" skip="0">)" << "\n";

      for (const auto& pair : tsr.results) {
        const auto& testName = pair.first;
        const auto& result = pair.second;

        oss << "\t" R"(<testcase name=")" << testName << R"(" )"
                    R"(time="0">)" << "\n";
        if (!result.passed) {
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

class test_adder__ {
public:
  test_adder__(const char* groupName, const char* methodName, TEST_METHOD method) {
    TestSuite::addTest(groupName, methodName, method);
  }
};

} // namespace sunfish

#endif // !defined(NDEBUG)

#endif // SUNFISH_TEST_TEST_HPP__
