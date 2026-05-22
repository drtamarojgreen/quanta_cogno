#include "api_handler.h"
#include <set>
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <map>
#include <deque>

// --- Rate Limiting Configuration ---
static const int MAX_REQUESTS_PER_WINDOW = 100;
static const std::chrono::seconds RATE_LIMIT_WINDOW(60);

// --- Rate Limiting State ---
static std::deque<std::chrono::steady_clock::time_point> request_timestamps;

// --- Caching Configuration ---
static const std::set<std::string> CACHEABLE_ENDPOINTS = {
    "getGene",
    "getGeneOntology"
};
static const std::chrono::seconds CACHE_TTL(300); // 5 minutes

// --- In-Memory Cache ---
// Key: Cache Key (endpoint + params)
// Value: Pair of (JSON Response, Expiration Time)
using CacheEntry = std::pair<JsonValue, std::chrono::steady_clock::time_point>;
static std::map<std::string, CacheEntry> api_cache;

// Endpoints that require at least one search parameter
static const std::set<std::string> BROAD_SEARCH_ENDPOINTS = {
    "getResearchAssociations",
    "getDrugGeneInteractions", 
    "getPolygeneticRiskScores"
};

// Forward declaration
JsonValue create_error_response(const std::string& message, const std::string& request_id, int error_code = 400);

// Helper function to generate a unique request ID
std::string generate_request_id() {
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(1000, 9999);

    std::stringstream ss;
    ss << "req_" << timestamp << "_" << distrib(gen);
    return ss.str();
}

// Helper function to generate a cache key
std::string generate_cache_key(const std::string& endpoint, const JsonValue& request) {
    return endpoint + ":" + request.serialize();
}

JsonValue process_api_request(const std::string& endpoint, const JsonValue& request) {
    const std::string request_id = generate_request_id();
    const auto start_time = std::chrono::high_resolution_clock::now();

    // --- Rate Limiting Check ---
    const auto now = std::chrono::steady_clock::now();
    while (!request_timestamps.empty() && now - request_timestamps.front() > RATE_LIMIT_WINDOW) {
        request_timestamps.pop_front();
    }

    if (request_timestamps.size() >= MAX_REQUESTS_PER_WINDOW) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        std::string err_msg = "Too many requests. Please try again later.";
        std::cout << "[ERROR] Request ID: " << request_id
                  << " | Status: Rate Limited"
                  << " | Duration: " << duration.count() << "ms"
                  << " | Message: " << err_msg << std::endl;
        return create_error_response(err_msg, request_id, 429);
    }

    request_timestamps.push_back(now);


    std::cout << "[INFO] Request ID: " << request_id
              << " | Timestamp: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()
              << " | Endpoint: " << endpoint
              << " | Parameters: " << request.serialize() << std::endl;

    // --- Cache Check ---
    if (CACHEABLE_ENDPOINTS.count(endpoint)) {
        std::string cache_key = generate_cache_key(endpoint, request);
        auto it = api_cache.find(cache_key);
        if (it != api_cache.end()) {
            // Check if expired
            if (std::chrono::steady_clock::now() < it->second.second) {
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end_time - start_time;
                std::cout << "[INFO] Request ID: " << request_id
                          << " | Status: Cache Hit"
                          << " | Duration: " << duration.count() << "ms" << std::endl;
                return it->second.first; // Return cached value
            }
        }
    }

    auto log_and_return_error = [&](const std::string& message, int error_code = 400) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        std::cout << "[ERROR] Request ID: " << request_id
                  << " | Status: Failure"
                  << " | Duration: " << duration.count() << "ms"
                  << " | Message: " << message << std::endl;
        return create_error_response(message, request_id, error_code);
    };

    // Check if this is a broad search endpoint that requires parameters
    if (BROAD_SEARCH_ENDPOINTS.find(endpoint) != BROAD_SEARCH_ENDPOINTS.end()) {
        if (request.object_value.find("parameters") == request.object_value.end()) {
            return log_and_return_error("Missing parameters object for endpoint: " + endpoint);
        }
        
        const JsonValue& parameters = request.object_value.at("parameters");
        
        if (parameters.type != JsonValue::OBJECT || parameters.object_value.empty()) {
            return log_and_return_error("Endpoint '" + endpoint + "' requires at least one search parameter to prevent overly broad queries.");
        }
        
        bool has_valid_parameter = false;
        for (const auto& param : parameters.object_value) {
            if (param.second.type != JsonValue::NIL) {
                if (param.second.type == JsonValue::STRING && !param.second.string_value.empty()) { has_valid_parameter = true; break; }
                else if (param.second.type == JsonValue::ARRAY && !param.second.array_value.empty()) { has_valid_parameter = true; break; }
                else if (param.second.type != JsonValue::STRING && param.second.type != JsonValue::ARRAY) { has_valid_parameter = true; break; }
            }
        }
        
        if (!has_valid_parameter) {
            return log_and_return_error("Endpoint '" + endpoint + "' requires at least one non-empty search parameter to prevent overly broad queries.");
        }
    }

    // Validate 'confidence_level' for 'getMentalHealthGenes' endpoint
    if (endpoint == "getMentalHealthGenes") {
        if (request.object_value.count("parameters")) {
            const auto& parameters = request.object_value.at("parameters").object_value;
            if (parameters.count("confidence_level")) {
                const auto& confidence_param = parameters.at("confidence_level");
                if (confidence_param.type == JsonValue::STRING) {
                    const std::string& value = confidence_param.string_value;
                    const std::set<std::string> valid_levels = {"high", "medium", "low", "all"};
                    if (valid_levels.find(value) == valid_levels.end()) {
                        return log_and_return_error("Invalid parameter: 'confidence_level' must be one of [high, medium, low, all].");
                    }
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    std::cout << "[INFO] Request ID: " << request_id
              << " | Status: Success"
              << " | Duration: " << duration.count() << "ms" << std::endl;

    JsonValue success_response = create_success_response("Request processed successfully for endpoint: " + endpoint);

    // --- Cache Store ---
    if (CACHEABLE_ENDPOINTS.count(endpoint)) {
        std::string cache_key = generate_cache_key(endpoint, request);
        auto expiration_time = std::chrono::steady_clock::now() + CACHE_TTL;
        api_cache[cache_key] = std::make_pair(success_response, expiration_time);
        std::cout << "[INFO] Request ID: " << request_id << " | Status: Stored in cache" << std::endl;
    }

    return success_response;
}

JsonValue create_error_response(const std::string& message, const std::string& request_id, int error_code) {
    JsonValue error_response = JsonValue::makeObject();
    JsonValue error_obj = JsonValue::makeObject();
    
    error_obj.object_value["code"] = JsonValue::makeNumber(error_code);
    error_obj.object_value["message"] = JsonValue::makeString(message);
    error_obj.object_value["requestId"] = JsonValue::makeString(request_id);
    
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
