#ifndef TESTING_FRAMEWORK_H
#define TESTING_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <exception>

// Global state for the test runner
struct TestStats {
    int passed = 0;
    int failed = 0;
};

// A simple assertion library
inline void _assert_true(bool condition, const std::string& expr, const std::string& file, int line, TestStats& stats) {
    if (condition) {
        stats.passed++;
    } else {
        stats.failed++;
        std::cerr << "\n  [FAIL] " << file << ":" << line << ": ASSERT_TRUE(" << expr << ")\n";
    }
}

template<typename T, typename U>
inline void _assert_equal(const T& a, const U& b, const std::string& expr_a, const std::string& expr_b, const std::string& file, int line, TestStats& stats) {
    if (a == b) {
        stats.passed++;
    } else {
        stats.failed++;
        std::cerr << "\n  [FAIL] " << file << ":" << line << ": ASSERT_EQUAL(" << expr_a << ", " << expr_b << ")\n";
        std::cerr << "    Expected: " << b << "\n";
        std::cerr << "    Actual:   " << a << "\n";
    }
}

#define ASSERT_TRUE(cond) _assert_true(cond, #cond, __FILE__, __LINE__, current_test_stats)
#define ASSERT_EQUAL(a, b) _assert_equal(a, b, #a, #b, __FILE__, __LINE__, current_test_stats)

// Test case runner
class TestRunner {
public:
    static TestRunner& get_instance() {
        static TestRunner instance;
        return instance;
    }

    using TestFunc = std::function<void(TestStats&)>;

    void add_test(const std::string& suite_name, const std::string& test_name, TestFunc func) {
        tests[suite_name].push_back({test_name, func});
    }

    int run_all() {
        std::cout << "Running all unit tests...\n";
        TestStats total_stats;
        for (const auto& suite : tests) {
            std::cout << "\n===========================================\n";
            std::cout << " SUITE: " << suite.first << "\n";
            std::cout << "===========================================\n";
            for (const auto& test : suite.second) {
                std::cout << "[RUN     ] " << test.first << std::flush;
                TestStats test_stats;
                try {
                    test.second(test_stats);
                    if (test_stats.failed == 0) {
                        std::cout << "\r[   OK   ] " << test.first << " (" << test_stats.passed << " assertions)" << std::endl;
                    } else {
                         std::cout << "\r[ FAILED ] " << test.first << " (" << test_stats.passed << " passed, " << test_stats.failed << " failed)" << std::endl;
                    }
                } catch (const std::exception& e) {
                    test_stats.failed++;
                    std::cerr << "\r[ FAILED ] " << test.first << " - Unhandled exception: " << e.what() << std::endl;
                } catch (...) {
                    test_stats.failed++;
                    std::cerr << "\r[ FAILED ] " << test.first << " - Unknown exception" << std::endl;
                }
                total_stats.passed += test_stats.passed;
                total_stats.failed += test_stats.failed;
            }
        }
        return total_stats.failed;
    }

private:
    TestRunner() = default;
    struct TestInfo { std::string name; TestFunc func; };
    std::map<std::string, std::vector<TestInfo>> tests;
};

// A helper struct to register tests using static initialization
struct TestRegisterer {
    TestRegisterer(const std::string& suite_name, const std::string& test_name, TestRunner::TestFunc func) {
        TestRunner::get_instance().add_test(suite_name, test_name, func);
    }
};

#define TEST_CASE(suite, name) \
    void test_##suite##_##name(TestStats& current_test_stats); \
    TestRegisterer register_##suite##_##name(#suite, #name, test_##suite##_##name); \
    void test_##suite##_##name(TestStats& current_test_stats)

#endif // TESTING_FRAMEWORK_H