#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

// @Card: quanta_cogno_structure_audit
// @Results missing_directories, missing_count

int main() {
    std::vector<std::string> required_dirs = {
        "src/api",
        "src/app",
        "src/core",
        "src/ui",
        "src/utils"
    };

    std::vector<std::string> missing;
    std::string root = "."; // Run from project root

    for (const auto& dir : required_dirs) {
        if (!fs::exists(root + "/" + dir)) {
            missing.push_back(dir);
        }
    }

    std::cout << "missing_count = " << missing.size() << std::endl;
    std::cout << "missing_directories = ";
    for (const auto& m : missing) std::cout << m << " ";
    std::cout << std::endl;

    return missing.empty() ? 0 : 1;
}
