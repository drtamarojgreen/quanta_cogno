#include "json_logic.h"
#include <cctype>
#include <sstream>

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
                os << '"' << kv.first << "":" << kv.second.serialize();
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

// A minimal parser that only supports flat objects with string/number/bool/null
JsonValue JsonValue::parse(const std::string& s) {
    JsonValue root = makeObject();
    size_t i = 0;
    auto skip = [&]{ while(i < s.size() && isspace(s[i])) i++; };
    skip();
    if(s[i] != '{') return root;
    i++; skip();
    while(i < s.size() && s[i] != '}') {
        // parse key
        if(s[i] != '"') break;
        i++;
        size_t start = i;
        while(i < s.size() && s[i] != '"') i++;
        std::string key = s.substr(start, i - start);
        i++; skip();
        if(s[i] != ':') break;
        i++; skip();
        // parse value
        JsonValue v;
        if(s[i] == '"') {
            i++;
            start = i;
            while(i < s.size() && s[i] != '"') i++;
            v = makeString(s.substr(start, i-start));
            i++; skip();
        } else if(isdigit(s[i]) || s[i]=='-') {
            start = i;
            while(i < s.size() && (isdigit(s[i])||s[i]=='.'||s[i]=='-')) i++;
            double num = atof(s.substr(start, i-start).c_str());
            v = makeNumber(num);
            skip();
        } else if(s.compare(i,4,"true")==0) { v = makeBool(true); i+=4; skip(); }
        else if(s.compare(i,5,"false")==0) { v = makeBool(false); i+=5; skip(); }
        else { v = makeNull(); }
        root.object_value[key] = v;
        if(s[i] == ',') { i++; skip(); } else break;
    }
    return root;
}