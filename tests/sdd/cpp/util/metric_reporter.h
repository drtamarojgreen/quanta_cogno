#ifndef METRIC_REPORTER_H
#define METRIC_REPORTER_H

#include <iostream>
#include <string>
#include <chrono>

namespace Sorrel::Sdd::Util {

class MetricReporter {
public:
    static void header(const std::string& title) {
        std::cout << "\n===========================================\n";
        std::cout << " " << title << "\n";
        std::cout << "===========================================\n";
    }

    static void report(const std::string& name, double value, const std::string& unit = "") {
        std::cout << "METRIC [" << name << "] = " << value;
        if (!unit.empty()) std::cout << " " << unit;
        std::cout << std::endl;
    }

    static void fact(const std::string& key, double value) {
        std::cout << key << " = " << value << std::endl;
    }
    
    static void fact(const std::string& key, const std::string& value) {
        std::cout << key << " = " << value << std::endl;
    }
};

class Timer {
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

} // namespace Sorrel::Sdd::Util

#endif // METRIC_REPORTER_H
