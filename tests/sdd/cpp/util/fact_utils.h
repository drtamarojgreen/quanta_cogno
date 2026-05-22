#ifndef FACT_UTILS_H
#define FACT_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

namespace Sorrel::Sdd::Util {

class FactReader {
public:
    static std::string trim(const std::string& s) {
        auto first = s.find_first_not_of(" \t\r\n");
        if (std::string::npos == first) return s;
        auto last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, (last - first + 1));
    }

    static std::map<std::string, std::string> readFacts(const std::string& filepath) {
        std::map<std::string, std::string> facts;
        std::ifstream file(filepath);
        if (!file.is_open()) return facts;

        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#' || line.find("Situation:") == 0) continue;

            size_t eq_pos = line.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = trim(line.substr(0, eq_pos));
                std::string value = trim(line.substr(eq_pos + 1));

                // Strip "Is ", "Needs ", "Results " prefixes
                const std::string prefixes[] = {"Is ", "Needs ", "Results "};
                for (const auto& p : prefixes) {
                    if (key.find(p) == 0) {
                        key = trim(key.substr(p.length()));
                        break;
                    }
                }
                facts[key] = value;
            }
        }
        return facts;
    }
};

} // namespace Sorrel::Sdd::Util

#endif // FACT_UTILS_H
