#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>
#include <memory>

namespace qc::io {

struct JsonValue;

using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue {
    std::variant<std::monostate, bool, double, std::string, JsonArray, JsonObject> data;

    bool is_null() const { return std::holds_alternative<std::monostate>(data); }
    bool is_bool() const { return std::holds_alternative<bool>(data); }
    bool is_number() const { return std::holds_alternative<double>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_array() const { return std::holds_alternative<JsonArray>(data); }
    bool is_object() const { return std::holds_alternative<JsonObject>(data); }

    bool as_bool() const { return std::get<bool>(data); }
    double as_number() const { return std::get<double>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    const JsonArray& as_array() const { return std::get<JsonArray>(data); }
    const JsonObject& as_object() const { return std::get<JsonObject>(data); }
};

struct ParseError {
    std::string message;
    size_t line;
    size_t column;
};

class JsonParser {
public:
    static std::variant<JsonValue, ParseError> parse(const std::string& input);

private:
    struct State {
        const std::string& input;
        size_t pos = 0;
        size_t line = 1;
        size_t column = 1;

        char peek() const { return pos < input.size() ? input[pos] : '\0'; }
        char consume();
        void skip_whitespace();
        ParseError error(const std::string& msg) const { return {msg, line, column}; }
    };

    static std::variant<JsonValue, ParseError> parse_value(State& state);
    static std::variant<JsonObject, ParseError> parse_object(State& state);
    static std::variant<JsonArray, ParseError> parse_array(State& state);
    static std::variant<std::string, ParseError> parse_string(State& state);
    static std::variant<double, ParseError> parse_number(State& state);
    static std::variant<bool, ParseError> parse_literal(State& state, const std::string& literal, bool value);
    static std::variant<std::monostate, ParseError> parse_null(State& state);
};

} // namespace qc::io

#endif // JSON_PARSER_H
