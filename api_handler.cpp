#include "api_handler.h"
#include <string>
#include <vector>

// Helper function to create a standardized error response
JsonValue make_error_response(int code, const std::string& message) {
    JsonValue error_obj = JsonValue::makeObject();
    error_obj.object_value["code"] = JsonValue::makeNumber(code);
    error_obj.object_value["message"] = JsonValue::makeString(message);

    JsonValue response = JsonValue::makeObject();
    response.object_value["error"] = error_obj;
    return response;
}

JsonValue process_api_request(const JsonValue& request) {
    // 1. Get the function name from the request
    if (request.type != JsonValue::OBJECT || request.object_value.find("name") == request.object_value.end()) {
        return make_error_response(400, "Request must be a JSON object with a 'name' field.");
    }
    const std::string& function_name = request.object_value.at("name").string_value;

    // 2. Define the list of endpoints that require at least one parameter
    const std::vector<std::string> broad_search_endpoints = {
        "getResearchAssociations",
        "getDrugGeneInteractions",
        "getPolygeneticRiskScores"
    };

    // 3. Check if the function is in our list
    bool needs_validation = false;
    for (const auto& endpoint : broad_search_endpoints) {
        if (function_name == endpoint) {
            needs_validation = true;
            break;
        }
    }

    // 4. If it needs validation, check the parameters
    if (needs_validation) {
        // Check if "parameters" field exists and is an object
        if (request.object_value.find("parameters") == request.object_value.end() ||
            request.object_value.at("parameters").type != JsonValue::OBJECT) {
             return make_error_response(400, "Endpoint requires a 'parameters' object.");
        }

        const auto& params = request.object_value.at("parameters").object_value;
        if (params.empty()) {
            return make_error_response(400, "At least one search parameter is required for this endpoint.");
        }
    }

    // 5. If validation passes or is not required, return a success response
    JsonValue response = JsonValue::makeObject();
    response.object_value["success"] = JsonValue::makeBool(true);
    return response;
}
