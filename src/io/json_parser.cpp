#include "json_parser.h"
#include <cctype>
#include <charconv>

namespace qc::io {

char JsonParser::State::consume() {
    char c = input[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void JsonParser::State::skip_whitespace() {
    while (pos < input.size() && std::isspace(input[pos])) {
        consume();
    }
}

std::variant<JsonValue, ParseError> JsonParser::parse(const std::string& input) {
    State state{input};
    state.skip_whitespace();
    auto result = parse_value(state);
    if (std::holds_alternative<ParseError>(result)) return result;
    
    state.skip_whitespace();
    if (state.pos < input.size()) {
        return state.error("Unexpected trailing character");
    }
    return result;
}

std::variant<JsonValue, ParseError> JsonParser::parse_value(State& state) {
    state.skip_whitespace();
    char c = state.peek();
    if (c == '{') {
        auto res = parse_object(state);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<JsonObject>(res)};
    }
    if (c == '[') {
        auto res = parse_array(state);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<JsonArray>(res)};
    }
    if (c == '"') {
        auto res = parse_string(state);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<std::string>(res)};
    }
    if (std::isdigit(c) || c == '-') {
        auto res = parse_number(state);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<double>(res)};
    }
    if (c == 't') {
        auto res = parse_literal(state, "true", true);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<bool>(res)};
    }
    if (c == 'f') {
        auto res = parse_literal(state, "false", false);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<bool>(res)};
    }
    if (c == 'n') {
        auto res = parse_null(state);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::monostate{}};
    }
    return state.error("Invalid JSON value");
}

std::variant<JsonObject, ParseError> JsonParser::parse_object(State& state) {
    state.consume(); // '{'
    JsonObject obj;
    state.skip_whitespace();
    if (state.peek() == '}') {
        state.consume();
        return obj;
    }

    while (true) {
        state.skip_whitespace();
        if (state.peek() != '"') return state.error("Expected object key");
        auto key_res = parse_string(state);
        if (std::holds_alternative<ParseError>(key_res)) return std::get<ParseError>(key_res);
        
        state.skip_whitespace();
        if (state.consume() != ':') return state.error("Expected ':' after key");
        
        auto val_res = parse_value(state);
        if (std::holds_alternative<ParseError>(val_res)) return std::get<ParseError>(val_res);
        
        obj[std::get<std::string>(key_res)] = std::get<JsonValue>(val_res);
        
        state.skip_whitespace();
        char c = state.consume();
        if (c == '}') break;
        if (c != ',') return state.error("Expected ',' or '}' in object");
    }
    return obj;
}

std::variant<JsonArray, ParseError> JsonParser::parse_array(State& state) {
    state.consume(); // '['
    JsonArray arr;
    state.skip_whitespace();
    if (state.peek() == ']') {
        state.consume();
        return arr;
    }

    while (true) {
        auto val_res = parse_value(state);
        if (std::holds_alternative<ParseError>(val_res)) return std::get<ParseError>(val_res);
        arr.push_back(std::get<JsonValue>(val_res));
        
        state.skip_whitespace();
        char c = state.consume();
        if (c == ']') break;
        if (c != ',') return state.error("Expected ',' or ']' in array");
    }
    return arr;
}

std::variant<std::string, ParseError> JsonParser::parse_string(State& state) {
    state.consume(); // '"'
    std::string s;
    while (state.pos < state.input.size()) {
        char c = state.consume();
        if (c == '"') return s;
        if (c == '\\') {
            if (state.pos >= state.input.size()) return state.error("Unexpected EOF in string escape");
            char esc = state.consume();
            switch (esc) {
                case '"': s += '"'; break;
                case '\\': s += '\\'; break;
                case '/': s += '/'; break;
                case 'b': s += '\b'; break;
                case 'f': s += '\f'; break;
                case 'n': s += '\n'; break;
                case 'r': s += '\r'; break;
                case 't': s += '\t'; break;
                default: return state.error("Invalid escape sequence");
            }
        } else {
            s += c;
        }
    }
    return state.error("Unexpected EOF in string");
}

std::variant<double, ParseError> JsonParser::parse_number(State& state) {
    size_t start = state.pos;
    if (state.peek() == '-') state.consume();
    while (std::isdigit(state.peek())) state.consume();
    if (state.peek() == '.') {
        state.consume();
        while (std::isdigit(state.peek())) state.consume();
    }
    if (state.peek() == 'e' || state.peek() == 'E') {
        state.consume();
        if (state.peek() == '+' || state.peek() == '-') state.consume();
        while (std::isdigit(state.peek())) state.consume();
    }
    
    double val;
    auto res = std::from_chars(state.input.data() + start, state.input.data() + state.pos, val);
    if (res.ec != std::errc{}) return state.error("Invalid number format");
    return val;
}

std::variant<bool, ParseError> JsonParser::parse_literal(State& state, const std::string& literal, bool value) {
    for (char c : literal) {
        if (state.consume() != c) return state.error("Expected literal: " + literal);
    }
    return value;
}

std::variant<std::monostate, ParseError> JsonParser::parse_null(State& state) {
    std::string literal = "null";
    for (char c : literal) {
        if (state.consume() != c) return state.error("Expected literal: null");
    }
    return std::monostate{};
}

} // namespace qc::io
