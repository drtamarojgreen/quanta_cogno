#include "testing_framework.h"
#include "json_logic.h"
#include "api_logic.h"
#include <fstream>
#include <cstdio> // For remove()

// Helper for comparing JsonValue, since it doesn't have operator==
bool are_equal(const JsonValue& a, const JsonValue& b) {
    if (a.type != b.type) return false;
    switch(a.type) {
        case JsonValue::STRING: return a.string_value == b.string_value;
        case JsonValue::NUMBER: return a.number_value == b.number_value;
        case JsonValue::BOOL: return a.bool_value == b.bool_value;
        case JsonValue::NIL: return true;
        // Not implementing deep comparison for object/array for this simple test
        case JsonValue::OBJECT: return a.object_value.size() == b.object_value.size();
        case JsonValue::ARRAY: return a.array_value.size() == b.array_value.size();
        default: return false;
    }
}

TEST_CASE(JsonLogic, CanCreateAndCheckTypes) {
    JsonValue str = JsonValue::makeString("hello");
    ASSERT_EQUAL(str.type, JsonValue::STRING);
    ASSERT_EQUAL(str.string_value, "hello");

    JsonValue num = JsonValue::makeNumber(123.45);
    ASSERT_EQUAL(num.type, JsonValue::NUMBER);
    ASSERT_EQUAL(num.number_value, 123.45);

    JsonValue b = JsonValue::makeBool(true);
    ASSERT_EQUAL(b.type, JsonValue::BOOL);
    ASSERT_EQUAL(b.bool_value, true);

    JsonValue n = JsonValue::makeNull();
    ASSERT_EQUAL(n.type, JsonValue::NIL);

    JsonValue obj = JsonValue::makeObject();
    ASSERT_EQUAL(obj.type, JsonValue::OBJECT);

    JsonValue arr = JsonValue::makeArray();
    ASSERT_EQUAL(arr.type, JsonValue::ARRAY);
}

TEST_CASE(JsonLogic, CanSerializeSimpleObject) {
    JsonValue obj = JsonValue::makeObject();
    obj.object_value["key"] = JsonValue::makeString("value");
    obj.object_value["num"] = JsonValue::makeNumber(10);

    std::string expected = "{\"key\":\"value\",\"num\":10}";
    std::string actual = obj.serialize();
    // Note: serialization doesn't guarantee key order, so this could fail.
    // A real test would parse the result and check keys/values.
    // For this scaffold, we assume order.
    ASSERT_EQUAL(actual, expected);
}

TEST_CASE(JsonLogic, CanParseSimpleObject) {
    std::string json_str = "{\"success\":true, \"data\": {\"id\": 123}}";
    JsonValue parsed = JsonValue::parse(json_str);

    ASSERT_EQUAL(parsed.type, JsonValue::OBJECT);
    ASSERT_EQUAL(parsed.object_value.count("success"), 1);
    ASSERT_EQUAL(parsed.object_value["success"].type, JsonValue::BOOL);
    ASSERT_EQUAL(parsed.object_value["success"].bool_value, true);
    ASSERT_EQUAL(parsed.object_value["data"].object_value["id"].number_value, 123);
}

TEST_CASE(ApiLogic, CanBuildRequest) {
    ApiConfig cfg;
    cfg.model = "test-model";
    cfg.prompt = "test-prompt";
    cfg.temperature = 0.5;
    cfg.max_tokens = 100;

    JsonValue request = build_request(cfg);
    ASSERT_EQUAL(request.object_value["model"].string_value, "test-model");
    ASSERT_EQUAL(request.object_value["prompt"].string_value, "test-prompt");
    ASSERT_EQUAL(request.object_value["temperature"].number_value, 0.5);
    ASSERT_EQUAL(request.object_value["max_tokens"].number_value, 100);
}

TEST_CASE(ApiLogic, CanSaveAndLoadFile) {
    const std::string filename = "test_saveload.json";
    JsonValue original = JsonValue::makeObject();
    original.object_value["test"] = JsonValue::makeString("data");

    ASSERT_TRUE(save_to_file(filename, original));
    JsonValue loaded = load_from_file(filename);
    ASSERT_TRUE(are_equal(original.object_value["test"], loaded.object_value["test"]));
    std::remove(filename.c_str());
}