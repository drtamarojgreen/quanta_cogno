#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../cpp/util/fact_utils.h"
#include "../cpp/util/metric_reporter.h"

using namespace Sorrel::Sdd::Util;

// @Card: validate_gates
// @Results total_gates_passed, critical_gate_failure_count
void validate_gates(const std::map<std::string, std::string>& facts) {
    MetricReporter::header("Audit: Reasoning Gate System");
    
    std::vector<std::string> gate_types = {
        "InterpretationGate", "ConstraintGate", "ToolSelectionGate", 
        "StrategyGate", "ScopeGate", "NavigationGate"
    };
    
    int passed_count = 0;
    int failed_count = 0;
    
    for (const auto& gate : gate_types) {
        if (facts.count(gate)) {
            int status = std::stoi(facts.at(gate));
            if (status == 1) passed_count++;
            else failed_count++;
            
            MetricReporter::fact(gate, status);
        }
    }
    
    MetricReporter::fact("total_gates_passed", passed_count);
    MetricReporter::fact("critical_gate_failure_count", failed_count);
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::string card_name = argv[1];
    auto facts = FactReader::readFacts("tests/sdd/facts/reasoning.facts");

    if (card_name == "validate_gates") {
        validate_gates(facts);
    }
    return 0;
}
