#include "yaml_parser.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace qc::io {

char YamlParser::State::consume() {
    char c = input[pos++];
    if (c == '\n') line++;
    return c;
}

void YamlParser::State::skip_whitespace_on_line() {
    while (pos < input.size() && input[pos] != '\n' && std::isspace(input[pos])) {
        pos++;
    }
}

size_t YamlParser::State::get_indent() {
    size_t indent = 0;
    size_t temp_pos = pos;
    while (temp_pos < input.size() && input[temp_pos] == ' ') {
        temp_pos++;
        indent++;
    }
    return indent;
}

std::variant<JsonValue, ParseError> YamlParser::parse(const std::string& input) {
    State state{input};
    while (state.peek() == '\n' || (std::isspace(state.peek()) && state.peek() != '\n')) state.consume();
    return parse_node(state, state.get_indent());
}

std::variant<JsonValue, ParseError> YamlParser::parse_node(State& state, size_t indent) {
    // Peek ahead to see if it's a list or map, WITHOUT consuming spaces
    size_t temp_pos = state.pos;
    while (temp_pos < state.input.size() && state.input[temp_pos] == ' ') temp_pos++;
    
    if (temp_pos < state.input.size() && state.input[temp_pos] == '-') {
        auto res = parse_list(state, indent);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<JsonArray>(res)};
    } else {
        auto res = parse_map(state, indent);
        if (std::holds_alternative<ParseError>(res)) return std::get<ParseError>(res);
        return JsonValue{std::get<JsonObject>(res)};
    }
}

std::variant<JsonObject, ParseError> YamlParser::parse_map(State& state, size_t indent) {
    JsonObject obj;
    while (state.pos < state.input.size()) {
        size_t current_indent = state.get_indent();
        
        // Handle empty lines or lines with only spaces
        size_t temp_pos = state.pos + current_indent;
        if (temp_pos < state.input.size() && state.input[temp_pos] == '\n') {
            state.pos = temp_pos;
            state.consume();
            continue;
        }

        if (current_indent < indent) break;
        if (current_indent > indent) return state.error("Invalid indentation in map: expected " + std::to_string(indent) + ", got " + std::to_string(current_indent));
        
        state.pos += current_indent;
        if (state.peek() == '-') break;

        std::string key;
        while (state.pos < state.input.size() && state.peek() != ':' && state.peek() != '\n') key += state.consume();
        key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), key.end());

        if (state.peek() != ':') return state.error("Expected ':' after key '" + key + "'");
        state.consume(); // ':'
        
        state.skip_whitespace_on_line();
        if (state.peek() == '\n' || state.pos >= state.input.size()) {
            if (state.peek() == '\n') state.consume();
            while (state.peek() == '\n') state.consume();
            
            size_t next_indent = state.get_indent();
            if (next_indent > indent) {
                auto val_res = parse_node(state, next_indent);
                if (std::holds_alternative<ParseError>(val_res)) return std::get<ParseError>(val_res);
                obj[key] = std::get<JsonValue>(val_res);
            } else {
                obj[key] = JsonValue{std::monostate{}};
            }
        } else {
            obj[key] = JsonValue{parse_scalar(state)};
            if (state.peek() == '\n') state.consume();
        }
    }
    return obj;
}

std::variant<JsonArray, ParseError> YamlParser::parse_list(State& state, size_t indent) {
    JsonArray arr;
    while (state.pos < state.input.size()) {
        size_t current_indent = state.get_indent();
        
        size_t temp_pos = state.pos + current_indent;
        if (temp_pos < state.input.size() && state.input[temp_pos] == '\n') {
            state.pos = temp_pos;
            state.consume();
            continue;
        }

        if (current_indent < indent) break;
        
        state.pos += current_indent;
        if (state.peek() != '-') break;
        state.consume(); // '-'
        state.skip_whitespace_on_line();
        
        if (state.peek() == '\n' || state.pos >= state.input.size()) {
            if (state.peek() == '\n') state.consume();
            while (state.peek() == '\n') state.consume();
            
            size_t next_indent = state.get_indent();
            auto val_res = parse_node(state, next_indent);
            if (std::holds_alternative<ParseError>(val_res)) return std::get<ParseError>(val_res);
            arr.push_back(std::get<JsonValue>(val_res));
        } else {
            arr.push_back(JsonValue{parse_scalar(state)});
            if (state.peek() == '\n') state.consume();
        }
    }
    return arr;
}

std::string YamlParser::parse_scalar(State& state) {
    std::string val;
    while (state.pos < state.input.size() && state.peek() != '\n') val += state.consume();
    val.erase(val.begin(), std::find_if(val.begin(), val.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    val.erase(std::find_if(val.rbegin(), val.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), val.end());
    return val;
}

} // namespace qc::io
