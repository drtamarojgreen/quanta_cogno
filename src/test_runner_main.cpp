#include "testing_framework.h"
#include <iostream>

int main() {
    std::cout << "Running unit test suite..." << std::endl;
    
    // Run all registered tests
    bool all_tests_passed = TestRunner::get_instance().run_all();
    
    if (all_tests_passed) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed!" << std::endl;
        return 1;
    }
}
