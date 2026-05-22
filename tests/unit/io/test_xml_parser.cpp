#include "io/xml_parser.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(XmlParser, ParsesSimpleElement) {
    std::string input = "<gene name=\"HTR2A\">Serotonin Receptor</gene>";
    auto node_opt = XmlParser::parse(input);
    
    ASSERT_TRUE(node_opt.has_value());
    auto node = node_opt.value();
    ASSERT_EQUAL(node->name, "gene");
    ASSERT_EQUAL(node->attributes.at("name"), "HTR2A");
    ASSERT_EQUAL(node->text, "Serotonin Receptor");
}

TEST_CASE(XmlParser, ParsesNestedElements) {
    std::string input = "<genome><chr id=\"1\"><gene>G1</gene></chr></genome>";
    auto node_opt = XmlParser::parse(input);
    
    ASSERT_TRUE(node_opt.has_value());
    auto node = node_opt.value();
    ASSERT_EQUAL(node->children.size(), 1);
    ASSERT_EQUAL(node->children[0]->name, "chr");
    ASSERT_EQUAL(node->children[0]->children[0]->name, "gene");
    ASSERT_EQUAL(node->children[0]->children[0]->text, "G1");
}

TEST_CASE(XmlParser, ParsesSelfClosingElement) {
    std::string input = "<marker id=\"rs123\" />";
    auto node_opt = XmlParser::parse(input);
    
    ASSERT_TRUE(node_opt.has_value());
    auto node = node_opt.value();
    ASSERT_EQUAL(node->name, "marker");
    ASSERT_EQUAL(node->attributes.at("id"), "rs123");
}
