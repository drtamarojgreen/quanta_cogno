#ifndef API_LOGIC_H
#define API_LOGIC_H
#include <string>
#include "json_logic.h"

// Configuration for the API call
struct ApiConfig {
    std::string model;
    std::string prompt;
    double temperature;
    int max_tokens;
    double top_p;
    double repeat_penalty;
};

// Build JSON request from config
JsonValue build_request(const ApiConfig& cfg);
// Simulate API call and produce a dummy JSON response
JsonValue simulate_api_call(const JsonValue& request);
// Save JSON value to file
bool save_to_file(const std::string& filename, const JsonValue& value);
// Load JSON value from file
JsonValue load_from_file(const std::string& filename);

#endif // API_LOGIC_H
