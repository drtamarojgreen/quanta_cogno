#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include "json_parser.h" // Reuse JsonValue for YAML representation
#include <string>
#include <vector>
#include <optional>

namespace qc::io {

class YamlParser {
public:
    static std::variant<JsonValue, ParseError> parse(const std::string& input);

private:
    struct State {
        const std::string& input;
        size_t pos = 0;
        size_t line = 1;

        char peek() const { return pos < input.size() ? input[pos] : '\0'; }
        char consume();
        void skip_whitespace_on_line();
        size_t get_indent();
        ParseError error(const std::string& msg) const { return {msg, line, 0}; }
    };

    static std::variant<JsonValue, ParseError> parse_node(State& state, size_t indent);
    static std::variant<JsonObject, ParseError> parse_map(State& state, size_t indent);
    static std::variant<JsonArray, ParseError> parse_list(State& state, size_t indent);
    static std::string parse_scalar(State& state);
};

} // namespace qc::io

#endif // YAML_PARSER_H
