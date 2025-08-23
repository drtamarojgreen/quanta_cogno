#include "../src/testing_framework.h"
#include "../src/flexible_json_logic.h"
#include <map>
#include <string>
#include <cstdlib> // For setenv

TEST_CASE(FlexibleJsonBdd, TemplatingFromContext) {
    // GIVEN a FlexibleJsonValue and a context map
    FlexibleJsonValue val;
    std::map<std::string, std::string> context;
    context["user_name"] = "Jules";
    context["user_role"] = "Engineer";

    // WHEN resolving a template string with placeholders for that context
    std::string tpl = "User: ${INPUT:user_name}, Role: ${CONFIG:user_role}";
    std::string result = val.resolveTemplate(tpl, context);

    // THEN the placeholders should be replaced with values from the context
    std::string expected = "User: Jules, Role: Engineer";
    ASSERT_EQUAL(result, expected);
}

TEST_CASE(FlexibleJsonBdd, TemplatingFromEnvironment) {
    // GIVEN a FlexibleJsonValue and an environment variable
    FlexibleJsonValue val;
    std::map<std::string, std::string> context; // empty context
    setenv("AUTH_TOKEN", "secret123", 1);

    // WHEN resolving a template string with a placeholder for the environment variable
    std::string tpl = "Token is ${ENV:AUTH_TOKEN}";
    std::string result = val.resolveTemplate(tpl, context);

    // THEN the placeholder should be replaced with the environment variable's value
    std::string expected = "Token is secret123";
    ASSERT_EQUAL(result, expected);
}
