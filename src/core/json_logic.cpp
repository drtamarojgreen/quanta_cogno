#include "json_logic.h"
#include <cctype>
#include <sstream>
#include <cstddef>

// Constructors and factory methods
JsonValue::JsonValue() : type(NIL), number_value(0), bool_value(false) {}
JsonValue JsonValue::makeString(const std::string& s) { JsonValue v; v.type = STRING; v.string_value = s; return v; }
JsonValue JsonValue::makeNumber(double d) { JsonValue v; v.type = NUMBER; v.number_value = d; return v; }
JsonValue JsonValue::makeBool(bool b) { JsonValue v; v.type = BOOL; v.bool_value = b; return v; }
JsonValue JsonValue::makeNull() { return JsonValue(); }
JsonValue JsonValue::makeObject() { JsonValue v; v.type = OBJECT; return v; }
JsonValue JsonValue::makeArray() { JsonValue v; v.type = ARRAY; return v; }

// Serialize JSON value into string
std::string JsonValue::serialize() const {
    std::ostringstream os;
    switch(type) {
        case STRING:
            os << '"';
            for(char c: string_value) {
                if(c == '"') os << "\\\"";
                else if(c == '\\') os << "\\\\";
                else os << c;
            }
            os << '"';
            break;
        case NUMBER:
            os << number_value;
            break;
        case BOOL:
            os << (bool_value ? "true" : "false");
            break;
        case NIL:
            os << "null";
            break;
        case OBJECT: {
            os << '{';
            bool first = true;
            for(auto& kv: object_value) {
                if(!first) os << ',';
                first = false;
                os << '"' << kv.first << "\":" << kv.second.serialize();
            }
            os << '}';
            break;
        }
        case ARRAY: {
            os << '[';
            bool first = true;
            for(auto& v: array_value) {
                if(!first) os << ',';
                first = false;
                os << v.serialize();
            }
            os << ']';
            break;
        }
    }
    return os.str();
}

// Forward declaration for recursive parsing
static JsonValue parse_value(const std::string& s, size_t& i);

// Helper to skip whitespace
static void skip_space(const std::string& s, size_t& i) {
    while (i < s.size() && isspace(s[i])) {
        i++;
    }
}

// Helper to parse a string
static std::string parse_string(const std::string& s, size_t& i) {
    i++; // Skip leading '"'
    size_t start = i;
    while (i < s.size() && s[i] != '"') {
        // This parser doesn't handle escaped quotes, for simplicity
        i++;
    }
    std::string str = s.substr(start, i - start);
    i++; // Skip trailing '"'
    return str;
}

// Helper to parse an object
static JsonValue parse_object(const std::string& s, size_t& i) {
    JsonValue obj = JsonValue::makeObject();
    i++; // Skip '{'
    skip_space(s, i);
    while (i < s.size() && s[i] != '}') {
        std::string key = parse_string(s, i);
        skip_space(s, i);
        i++; // Skip ':'
        skip_space(s, i);
        obj.object_value[key] = parse_value(s, i);
        skip_space(s, i);
        if (s[i] == ',') {
            i++;
            skip_space(s, i);
        }
    }
    i++; // Skip '}'
    return obj;
}

// Main recursive value parser
static JsonValue parse_value(const std::string& s, size_t& i) {
    skip_space(s, i);
    switch (s[i]) {
        case '"':
            return JsonValue::makeString(parse_string(s, i));
        case '{':
            return parse_object(s, i);
        case 't':
            i += 4; // "true"
            return JsonValue::makeBool(true);
        case 'f':
            i += 5; // "false"
            return JsonValue::makeBool(false);
        case 'n':
            i += 4; // "null"
            return JsonValue::makeNull();
        default: // Number
            size_t start = i;
            while (i < s.size() && (isdigit(s[i]) || s[i] == '.' || s[i] == '-')) {
                i++;
            }
            double num = std::stod(s.substr(start, i - start));
            return JsonValue::makeNumber(num);
    }
}

JsonValue JsonValue::parse(const std::string& s) {
    size_t i = 0;
    return parse_value(s, i);
}
