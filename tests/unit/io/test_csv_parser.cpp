#include "io/csv_parser.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(CsvParser, ParsesSimpleCsv) {
    std::string input = "id,name,value\n1,gene1,0.5\n2,gene2,0.8";
    auto data = CsvParser::parse(input);
    
    ASSERT_EQUAL(data.size(), 3);
    ASSERT_EQUAL(data[0][0], "id");
    ASSERT_EQUAL(data[1][1], "gene1");
    ASSERT_EQUAL(data[2][2], "0.8");
}

TEST_CASE(CsvParser, ParsesQuotedFields) {
    std::string input = "id,desc\n1,\"Gene with, comma\"\n2,\"Gene with \"\"quotes\"\"\"";
    auto data = CsvParser::parse(input);
    
    ASSERT_EQUAL(data.size(), 3);
    ASSERT_EQUAL(data[1][1], "Gene with, comma");
    ASSERT_EQUAL(data[2][1], "Gene with \"quotes\"");
}

TEST_CASE(CsvParser, ParsesToMaps) {
    std::string input = "id,expr\nrs1,0.1\nrs2,0.9";
    auto data = CsvParser::parse_to_maps(input);
    
    ASSERT_EQUAL(data.size(), 2);
    ASSERT_EQUAL(data[0].at("id"), "rs1");
    ASSERT_EQUAL(data[1].at("expr"), "0.9");
}
