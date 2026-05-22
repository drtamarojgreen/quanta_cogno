#include "utils/testing_framework.h"
#include <iostream>

int main() {
    std::cout << "Running unit test suite..." << std::endl;
    
    // Run all registered tests
    int failures = TestRunner::get_instance().run_all();
    
    if (failures == 0) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed! Total failures: " << failures << std::endl;
        return 1;
    }
}
