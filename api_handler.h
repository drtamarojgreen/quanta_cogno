#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "json_logic.h"
#include <string>

// Process API requests with validation for mandatory search parameters
JsonValue process_api_request(const std::string& endpoint, const JsonValue& request);

// Helper function to create standardized error responses
JsonValue create_error_response(const std::string& message, int error_code = 400);

// Helper function to create success responses
JsonValue create_success_response(const std::string& message);

#endif // API_HANDLER_H
