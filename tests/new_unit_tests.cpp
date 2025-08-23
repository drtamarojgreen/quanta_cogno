#include "../src/testing_framework.h"
#include "../src/api_logic.h"

TEST_CASE(ApiLogic, CanBuildRequestWithEmptyConfig) {
    ApiConfig cfg;
    // cfg is empty, should use default values, which are empty strings or 0
    JsonValue request = build_request(cfg);
    ASSERT_EQUAL(request.object_value["model"].string_value, "");
    ASSERT_EQUAL(request.object_value["prompt"].string_value, "");
    // Assuming default for number is 0.0, might need to check api_logic.cpp
    ASSERT_EQUAL(request.object_value["temperature"].number_value, 0.0);
    ASSERT_EQUAL(request.object_value["max_tokens"].number_value, 0);
}
