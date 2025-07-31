#ifndef JSON_LOGIC_H
#define JSON_LOGIC_H
#include <string>
#include <map>
#include <vector>

// A simple JSON value type
class JsonValue {
public:
    enum Type { STRING, NUMBER, OBJECT, ARRAY, BOOL, NIL };
    Type type;
    std::string string_value;
    double number_value;
    bool bool_value;
    std::map<std::string, JsonValue> object_value;
    std::vector<JsonValue> array_value;

    JsonValue();
    static JsonValue makeString(const std::string&);
    static JsonValue makeNumber(double);
    static JsonValue makeBool(bool);
    static JsonValue makeNull();
    static JsonValue makeObject();
    static JsonValue makeArray();

    std::string serialize() const;
    static JsonValue parse(const std::string&);
};

#endif // JSON_LOGIC_H
