#include "api_logic.h"
#include <fstream>

JsonValue build_request(const ApiConfig& cfg) {
    JsonValue root = JsonValue::makeObject();
    root.object_value["model"] = JsonValue::makeString(cfg.model);
    root.object_value["prompt"] = JsonValue::makeString(cfg.prompt);
    root.object_value["temperature"] = JsonValue::makeNumber(cfg.temperature);
    root.object_value["max_tokens"] = JsonValue::makeNumber(cfg.max_tokens);
    root.object_value["top_p"] = JsonValue::makeNumber(cfg.top_p);
    root.object_value["repeat_penalty"] = JsonValue::makeNumber(cfg.repeat_penalty);
    return root;
}

JsonValue simulate_api_call(const JsonValue& request) {
    // Dummy response: echo prompt and add a result string
    JsonValue resp = JsonValue::makeObject();
    resp.object_value["success"] = JsonValue::makeBool(true);
    auto req_prompt = request.object_value.at("prompt").string_value;
    resp.object_value["response"] = JsonValue::makeString(
        "[SIMULATED RESPONSE] Based on prompt: " + req_prompt
    );
    return resp;
}

bool save_to_file(const std::string& filename, const JsonValue& value) {
    std::ofstream ofs(filename);
    if(!ofs) return false;
    ofs << value.serialize();
    return true;
}

JsonValue load_from_file(const std::string& filename) {
    std::ifstream ifs(filename);
    if(!ifs) return JsonValue::makeNull();
    std::string content((std::istreambuf_iterator<char>(ifs)), {});
    return JsonValue::parse(content);
}