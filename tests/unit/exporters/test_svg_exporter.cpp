#include "io/svg_exporter.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(SvgExporter, GeneratesValidXml) {
    std::vector<SvgCircle> circles = {{{50, 50}, 10, "red"}};
    std::vector<SvgLine> lines = {{{0, 0}, {100, 100}, "black", 2}};
    
    std::string svg = SvgExporter::export_to_string(100, 100, circles, lines);
    
    ASSERT_TRUE(svg.find("<svg") != std::string::npos);
    ASSERT_TRUE(svg.find("<circle") != std::string::npos);
    ASSERT_TRUE(svg.find("fill=\"red\"") != std::string::npos);
    ASSERT_TRUE(svg.find("</svg>") != std::string::npos);
}
