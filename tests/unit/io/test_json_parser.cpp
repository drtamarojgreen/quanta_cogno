#include "io/json_parser.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(JsonParser, ParsesNull) {
    auto res = JsonParser::parse("null");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res));
    ASSERT_TRUE(std::get<JsonValue>(res).is_null());
}

TEST_CASE(JsonParser, ParsesBool) {
    auto res_t = JsonParser::parse("true");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res_t));
    ASSERT_TRUE(std::get<JsonValue>(res_t).as_bool() == true);

    auto res_f = JsonParser::parse("false");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res_f));
    ASSERT_TRUE(std::get<JsonValue>(res_f).as_bool() == false);
}

TEST_CASE(JsonParser, ParsesNumber) {
    auto res = JsonParser::parse("123.45");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res));
    ASSERT_EQUAL(std::get<JsonValue>(res).as_number(), 123.45);
}

TEST_CASE(JsonParser, ParsesString) {
    auto res = JsonParser::parse("\"hello world\"");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res));
    ASSERT_EQUAL(std::get<JsonValue>(res).as_string(), "hello world");
}

TEST_CASE(JsonParser, ParsesObject) {
    auto res = JsonParser::parse("{\"key\": \"value\", \"num\": 42}");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res));
    auto obj = std::get<JsonValue>(res).as_object();
    ASSERT_EQUAL(obj.at("key").as_string(), "value");
    ASSERT_EQUAL(obj.at("num").as_number(), 42.0);
}

TEST_CASE(JsonParser, ParsesArray) {
    auto res = JsonParser::parse("[1, 2, \"three\"]");
    ASSERT_TRUE(std::holds_alternative<JsonValue>(res));
    auto arr = std::get<JsonValue>(res).as_array();
    ASSERT_EQUAL(arr.size(), 3);
    ASSERT_EQUAL(arr[0].as_number(), 1.0);
    ASSERT_EQUAL(arr[2].as_string(), "three");
}

TEST_CASE(JsonParser, ReportsError) {
    auto res = JsonParser::parse("{\"key\": }");
    ASSERT_TRUE(std::holds_alternative<ParseError>(res));
    auto err = std::get<ParseError>(res);
    ASSERT_EQUAL(err.line, 1);
}
