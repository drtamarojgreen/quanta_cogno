#include "io/json_parser.h"
#include "core/simulation_engine.h"
#include "io/svg_exporter.h"
#include "utils/testing_framework.h"

using namespace qc;

TEST_CASE(E2E, FullPipeline_JsonToSvg) {
    // 1. INPUT (JSON)
    std::string json_input = "{\"genes\": [{\"id\": \"G1\", \"expression\": 0.1, \"variants\": [{\"id\": \"v1\", \"impact\": 0.8}]}]}";
    auto parse_res = io::JsonParser::parse(json_input);
    ASSERT_TRUE(std::holds_alternative<io::JsonValue>(parse_res));
    
    // 2. PROCESS (Simulation)
    core::SimulationEngine engine;
    auto genes_arr = std::get<io::JsonValue>(parse_res).as_object().at("genes").as_array();
    for (const auto& gj : genes_arr) {
        auto obj = gj.as_object();
        core::Gene g;
        g.id = obj.at("id").as_string();
        g.expression_level = obj.at("expression").as_number();
        engine.add_gene(g);
    }
    
    engine.step(10.0);
    
    // 3. OUTPUT (SVG)
    std::vector<io::SvgCircle> circles;
    for (const auto& [id, gene] : engine.get_genes()) {
        circles.push_back({{100, 100}, gene.expression_level * 50, "blue"});
    }
    
    std::string svg = io::SvgExporter::export_to_string(200, 200, circles, {});
    ASSERT_TRUE(svg.find("<circle") != std::string::npos);
}
