#include "xml_parser.h"
#include <algorithm>

namespace qc::io {

void XmlParser::State::skip_whitespace() {
    while (pos < input.size() && std::isspace(input[pos])) {
        pos++;
    }
}

bool XmlParser::State::match(const std::string& s) {
    if (pos + s.size() > input.size()) return false;
    if (input.compare(pos, s.size(), s) == 0) {
        pos += s.size();
        return true;
    }
    return false;
}

std::optional<std::shared_ptr<XmlNode>> XmlParser::parse(const std::string& input) {
    State state{input};
    state.skip_whitespace();
    if (!state.match("<")) return std::nullopt;
    return parse_element(state);
}

std::shared_ptr<XmlNode> XmlParser::parse_element(State& state) {
    std::string name = parse_name(state);
    auto node = XmlNode::make(name);

    // Parse attributes
    while (state.pos < state.input.size() && state.peek() != '>' && state.peek() != '/') {
        state.skip_whitespace();
        if (state.peek() == '>' || state.peek() == '/') break;
        std::string attr_name = parse_name(state);
        state.match("=");
        if (state.consume() != '"') break;
        std::string attr_val;
        while (state.peek() != '"') attr_val += state.consume();
        state.consume(); // '"'
        node->attributes[attr_name] = attr_val;
        state.skip_whitespace();
    }

    if (state.match("/>")) return node;
    state.match(">");

    // Parse children and text
    while (state.pos < state.input.size()) {
        state.skip_whitespace();
        if (state.match("</")) {
            parse_name(state);
            state.match(">");
            break;
        } else if (state.peek() == '<') {
            state.consume(); // '<'
            node->children.push_back(parse_element(state));
        } else {
            node->text += parse_text(state);
        }
    }

    return node;
}

std::string XmlParser::parse_name(State& state) {
    std::string name;
    while (state.pos < state.input.size() && (std::isalnum(state.peek()) || state.peek() == '_' || state.peek() == ':')) {
        name += state.consume();
    }
    return name;
}

std::string XmlParser::parse_text(State& state) {
    std::string text;
    while (state.pos < state.input.size() && state.peek() != '<') {
        text += state.consume();
    }
    return text;
}

} // namespace qc::io
