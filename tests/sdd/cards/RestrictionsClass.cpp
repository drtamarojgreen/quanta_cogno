#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "../cpp/util/fact_utils.h"
#include "../cpp/util/metric_reporter.h"

namespace fs = std::filesystem;
using namespace Sorrel::Sdd::Util;

// @Card: audit_structure
// @Results hardcoded_path_count, missing_namespace_count, over_limit_file_count
void audit_structure(const std::map<std::string, std::string>& facts) {
    MetricReporter::header("Audit: Structural Restrictions");
    
    int max_lines = std::stoi(facts.at("max_file_lines"));
    std::string req_ns = facts.at("required_namespace");
    
    int hardcoded_path_count = 0;
    int missing_namespace_count = 0;
    int over_limit_file_count = 0;
    int total_files_audited = 0;

    for (const auto& entry : fs::recursive_directory_iterator("src")) {
        if (entry.is_regular_file() && (entry.path().extension() == ".cpp" || entry.path().extension() == ".h")) {
            total_files_audited++;
            std::ifstream f(entry.path());
            std::string line;
            int line_count = 0;
            bool ns_found = false;
            
            while (std::getline(f, line)) {
                line_count++;
                
                // Very basic heuristic for hardcoded paths (e.g., "C:/", "./", "../")
                // Excluding includes and specific safe strings
                if (line.find("\"C:/") != std::string::npos || 
                    line.find("\"./") != std::string::npos || 
                    line.find("\"../") != std::string::npos) {
                    if (line.find("#include") == std::string::npos) {
                        hardcoded_path_count++;
                    }
                }
                
                if (line.find("namespace " + req_ns) != std::string::npos) {
                    ns_found = true;
                }
            }
            
            if (line_count > max_lines) over_limit_file_count++;
            if (!ns_found) missing_namespace_count++;
        }
    }

    MetricReporter::fact("total_files_audited", total_files_audited);
    MetricReporter::fact("hardcoded_path_count", hardcoded_path_count);
    MetricReporter::fact("missing_namespace_count", missing_namespace_count);
    MetricReporter::fact("over_limit_file_count", over_limit_file_count);
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::string card_name = argv[1];
    auto facts = FactReader::readFacts("tests/sdd/facts/restrictions.facts");

    if (card_name == "audit_structure") {
        audit_structure(facts);
    }
    return 0;
}
