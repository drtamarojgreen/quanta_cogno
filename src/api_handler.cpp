#include "api_handler.h"
#include <set>

// Endpoints that require at least one search parameter
static const std::set<std::string> BROAD_SEARCH_ENDPOINTS = {
    "getResearchAssociations",
    "getDrugGeneInteractions", 
    "getPolygeneticRiskScores"
};

JsonValue process_api_request(const std::string& endpoint, const JsonValue& request) {
    // Check if this is a broad search endpoint that requires parameters
    if (BROAD_SEARCH_ENDPOINTS.find(endpoint) != BROAD_SEARCH_ENDPOINTS.end()) {
        // Check if request has parameters
        if (request.object_value.find("parameters") == request.object_value.end()) {
            return create_error_response("Missing parameters object for endpoint: " + endpoint);
        }
        
        const JsonValue& parameters = request.object_value.at("parameters");
        
        // Check if parameters object is empty or has no meaningful parameters
        if (parameters.type != JsonValue::OBJECT || parameters.object_value.empty()) {
            return create_error_response(
                "Endpoint '" + endpoint + "' requires at least one search parameter to prevent overly broad queries."
            );
        }
        
        // Check if all parameters are null or empty
        bool has_valid_parameter = false;
        for (const auto& param : parameters.object_value) {
            if (param.second.type != JsonValue::NIL) {
                // For string parameters, check if they're not empty
                if (param.second.type == JsonValue::STRING && !param.second.string_value.empty()) {
                    has_valid_parameter = true;
                    break;
                }
                // For array parameters, check if they're not empty
                else if (param.second.type == JsonValue::ARRAY && !param.second.array_value.empty()) {
                    has_valid_parameter = true;
                    break;
                }
                // For other non-null types, consider them valid
                else if (param.second.type != JsonValue::STRING && param.second.type != JsonValue::ARRAY) {
                    has_valid_parameter = true;
                    break;
                }
            }
        }
        
        if (!has_valid_parameter) {
            return create_error_response(
                "Endpoint '" + endpoint + "' requires at least one non-empty search parameter to prevent overly broad queries."
            );
        }
    }
    
    // If validation passes, return success
    return create_success_response("Request processed successfully for endpoint: " + endpoint);
}

JsonValue create_error_response(const std::string& message, int error_code) {
    JsonValue error_response = JsonValue::makeObject();
    JsonValue error_obj = JsonValue::makeObject();
    
    error_obj.object_value["code"] = JsonValue::makeNumber(error_code);
    error_obj.object_value["message"] = JsonValue::makeString(message);
    
    error_response.object_value["error"] = error_obj;
    error_response.object_value["success"] = JsonValue::makeBool(false);
    
    return error_response;
}

JsonValue create_success_response(const std::string& message) {
    JsonValue success_response = JsonValue::makeObject();
    
    success_response.object_value["success"] = JsonValue::makeBool(true);
    success_response.object_value["message"] = JsonValue::makeString(message);
    
    return success_response;
}
