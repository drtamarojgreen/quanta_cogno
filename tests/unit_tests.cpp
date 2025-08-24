#include "../testing_framework.h"
#include "../json_logic.h"
#include "../api_logic.h"
#include "../api_handler.h"
#include "../flexible_json_logic.h"
#include <fstream>
#include <cstdio> // For remove()
#include <cstdlib> // For setenv

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

// Tests for mandatory search parameters feature
TEST_CASE(ApiHandler, RejectsGetResearchAssociationsWithNoParameters) {
    JsonValue request = JsonValue::makeObject();
    JsonValue empty_params = JsonValue::makeObject();
    request.object_value["parameters"] = empty_params;
    
    JsonValue response = process_api_request("getResearchAssociations", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("requires at least one search parameter") != std::string::npos);
}

TEST_CASE(ApiHandler, RejectsGetDrugGeneInteractionsWithNoParameters) {
    JsonValue request = JsonValue::makeObject();
    JsonValue empty_params = JsonValue::makeObject();
    request.object_value["parameters"] = empty_params;
    
    JsonValue response = process_api_request("getDrugGeneInteractions", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("requires at least one search parameter") != std::string::npos);
}

TEST_CASE(ApiHandler, RejectsGetPolygeneticRiskScoresWithNoParameters) {
    JsonValue request = JsonValue::makeObject();
    JsonValue empty_params = JsonValue::makeObject();
    request.object_value["parameters"] = empty_params;
    
    JsonValue response = process_api_request("getPolygeneticRiskScores", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("requires at least one search parameter") != std::string::npos);
}

TEST_CASE(ApiHandler, AcceptsBroadSearchEndpointWithValidParameter) {
    JsonValue request = JsonValue::makeObject();
    JsonValue params = JsonValue::makeObject();
    params.object_value["gene_ids"] = JsonValue::makeString("COMT");
    request.object_value["parameters"] = params;
    
    JsonValue response = process_api_request("getResearchAssociations", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, true);
    ASSERT_TRUE(response.object_value["message"].string_value.find("Request processed successfully") != std::string::npos);
}

TEST_CASE(FlexibleJsonLogic, CanBeTreatedAsJsonValue) {
    FlexibleJsonValue flex_val;
    JsonValue* base_ptr = &flex_val;

    // Check that the object can be pointed to by a base class pointer,
    // confirming the inheritance relationship.
    ASSERT_TRUE(base_ptr != nullptr);

    // A default-constructed JsonValue (and by extension FlexibleJsonValue)
    // should have the type NIL.
    ASSERT_EQUAL(base_ptr->type, JsonValue::NIL);
}

TEST_CASE(FlexibleJsonLogic, TemplateResolution) {
    FlexibleJsonValue val;
    std::map<std::string, std::string> context;
    context["user_id"] = "12345";
    context["timeout"] = "100";

    // Test ENV variable
    setenv("API_KEY", "test-key", 1);
    std::string t1 = "key=${ENV:API_KEY}";
    std::string r1 = val.resolveTemplate(t1, context);
    ASSERT_EQUAL(r1, "key=test-key");

    // Test INPUT variable
    std::string t2 = "user=${INPUT:user_id}";
    std::string r2 = val.resolveTemplate(t2, context);
    ASSERT_EQUAL(r2, "user=12345");

    // Test CONFIG variable
    std::string t3 = "timeout=${CONFIG:timeout}";
    std::string r3 = val.resolveTemplate(t3, context);
    ASSERT_EQUAL(r3, "timeout=100");

    // Test CONFIG with default
    std::string t4 = "limit=${CONFIG:limit|50}";
    std::string r4 = val.resolveTemplate(t4, context);
    ASSERT_EQUAL(r4, "limit=50");

    // Test multiple variables
    std::string t5 = "user=${INPUT:user_id}, key=${ENV:API_KEY}, timeout=${CONFIG:timeout}";
    std::string r5 = val.resolveTemplate(t5, context);
    ASSERT_EQUAL(r5, "user=12345, key=test-key, timeout=100");

    // Test no variables
    std::string t6 = "this is a plain string";
    std::string r6 = val.resolveTemplate(t6, context);
    ASSERT_EQUAL(r6, "this is a plain string");
}

TEST_CASE(ApiHandler, DoesNotAffectUnrelatedEndpoints) {
    JsonValue request = JsonValue::makeObject();
    JsonValue params = JsonValue::makeObject();
    params.object_value["gene"] = JsonValue::makeString("COMT");
    request.object_value["parameters"] = params;
    
    JsonValue response = process_api_request("getGene", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, true);
    ASSERT_TRUE(response.object_value["message"].string_value.find("Request processed successfully") != std::string::npos);
}

TEST_CASE(ApiHandler, RejectsRequestWithMissingParametersObject) {
    JsonValue request = JsonValue::makeObject();
    // No parameters object at all
    
    JsonValue response = process_api_request("getResearchAssociations", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("Missing parameters object") != std::string::npos);
}

TEST_CASE(ApiHandler, RejectsRequestWithOnlyNullParameters) {
    JsonValue request = JsonValue::makeObject();
    JsonValue params = JsonValue::makeObject();
    params.object_value["gene_ids"] = JsonValue::makeNull();
    params.object_value["condition"] = JsonValue::makeNull();
    request.object_value["parameters"] = params;
    
    JsonValue response = process_api_request("getDrugGeneInteractions", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("requires at least one non-empty search parameter") != std::string::npos);
}

TEST_CASE(ApiHandler, RejectsRequestWithOnlyEmptyStringParameters) {
    JsonValue request = JsonValue::makeObject();
    JsonValue params = JsonValue::makeObject();
    params.object_value["condition"] = JsonValue::makeString("");
    request.object_value["parameters"] = params;
    
    JsonValue response = process_api_request("getPolygeneticRiskScores", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, false);
    ASSERT_TRUE(response.object_value.count("error") > 0);
    ASSERT_TRUE(response.object_value["error"].object_value["message"].string_value.find("requires at least one non-empty search parameter") != std::string::npos);
}

TEST_CASE(ApiHandler, AcceptsRequestWithValidArrayParameter) {
    JsonValue request = JsonValue::makeObject();
    JsonValue params = JsonValue::makeObject();
    JsonValue gene_array = JsonValue::makeArray();
    gene_array.array_value.push_back(JsonValue::makeString("COMT"));
    gene_array.array_value.push_back(JsonValue::makeString("HTR2A"));
    params.object_value["gene_ids"] = gene_array;
    request.object_value["parameters"] = params;
    
    JsonValue response = process_api_request("getDrugGeneInteractions", request);
    
    ASSERT_EQUAL(response.object_value["success"].bool_value, true);
    ASSERT_TRUE(response.object_value["message"].string_value.find("Request processed successfully") != std::string::npos);
}
