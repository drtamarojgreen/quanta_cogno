#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "json_logic.h"

/**
 * @brief Processes a simulated API request.
 *
 * This function takes a JSON request, validates it according to the API definition,
 * and returns a JSON response. It is the core of the simulated API logic.
 *
 * @param request A JsonValue object representing the API request.
 *                The request should have a "name" field specifying the API function
 *                and a "parameters" field containing the arguments.
 * @return A JsonValue object representing the API response. This will be a
 *         success object or a standardized error object.
 */
JsonValue process_api_request(const JsonValue& request);

#endif // API_HANDLER_H
