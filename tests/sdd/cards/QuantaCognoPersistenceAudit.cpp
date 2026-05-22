#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// @Card: quanta_cogno_persistence_audit
// @Results state_file_found, cache_dir_found

int main() {
    std::string root = ".";
    bool state_found = fs::exists(root + "/app_state.json");
    bool cache_found = fs::exists(root + "/cache") && fs::is_directory(root + "/cache");

    std::cout << "state_file_found = " << (state_found ? "true" : "false") << std::endl;
    std::cout << "cache_dir_found = " << (cache_found ? "true" : "false") << std::endl;

    return (state_found && cache_found) ? 0 : 1;
}
