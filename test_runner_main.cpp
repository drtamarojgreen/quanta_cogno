#include "testing_framework.h"

int main() {
    // This will execute all registered tests and return the number of failures.
    return TestRunner::get_instance().run_all();
}
