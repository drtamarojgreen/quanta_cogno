#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>

namespace qc::io {

struct XmlNode {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string text;
    std::vector<std::shared_ptr<XmlNode>> children;

    static std::shared_ptr<XmlNode> make(const std::string& name) {
        auto node = std::make_shared<XmlNode>();
        node->name = name;
        return node;
    }
};

class XmlParser {
public:
    static std::optional<std::shared_ptr<XmlNode>> parse(const std::string& input);

private:
    struct State {
        const std::string& input;
        size_t pos = 0;

        char peek() const { return pos < input.size() ? input[pos] : '\0'; }
        char consume() { return input[pos++]; }
        void skip_whitespace();
        bool match(const std::string& s);
    };

    static std::shared_ptr<XmlNode> parse_element(State& state);
    static std::string parse_name(State& state);
    static std::string parse_text(State& state);
};

} // namespace qc::io

#endif // XML_PARSER_H
