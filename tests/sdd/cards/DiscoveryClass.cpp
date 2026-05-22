#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../cpp/util/fact_utils.h"
#include "../cpp/util/metric_reporter.h"

namespace fs = std::filesystem;
using namespace Sorrel::Sdd::Util;

// @Card: discover_environment
// @Results project_root_discovered = 1
void discover_environment(const std::map<std::string, std::string>& facts) {
    MetricReporter::header("Discovery: Environment");
    
    bool root_found = fs::exists("DESIGN.md") && fs::exists("src/main.cpp");
    
    MetricReporter::fact("project_root_discovered", root_found ? 1 : 0);
    MetricReporter::fact("source_dir_found", fs::exists("src") ? 1 : 0);
    MetricReporter::fact("tests_dir_found", fs::exists("tests") ? 1 : 0);
    
    // Discovery of directories as required by SORREL rule
    if (root_found) {
        MetricReporter::fact("sdd_path", fs::absolute("tests/sdd").string());
        MetricReporter::fact("facts_path", fs::absolute("tests/sdd/facts").string());
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: DiscoveryClass <logical_card_name>" << std::endl;
        return 1;
    }

    std::string card_name = argv[1];
    auto facts = FactReader::readFacts("tests/sdd/facts/environment.facts");

    if (card_name == "discover_environment") {
        discover_environment(facts);
    } else {
        std::cerr << "Unknown logical card: " << card_name << std::endl;
        return 1;
    }

    return 0;
}
