#include "core/simulation_engine.h"
#include "utils/testing_framework.h"

using namespace qc::core;

TEST_CASE(BDD_Simulation, SnpImpactScenario) {
    // GIVEN
    SimulationEngine engine;
    Gene g{"HTR2A", 0.5, {{"rs6311", 0.9}}};
    engine.add_gene(g);
    
    // WHEN
    for (int i = 0; i < 50; ++i) {
        engine.step(1.0);
    }
    
    // THEN
    double final_expr = engine.get_genes().at("HTR2A").expression_level;
    ASSERT_TRUE(final_expr > 0.5);
}
