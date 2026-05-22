#include "core/simulation_engine.h"
#include "utils/testing_framework.h"

using namespace qc::core;

TEST_CASE(SimulationEngine, SimulatesExpressionDrift) {
    SimulationEngine engine;
    Gene g;
    g.id = "TEST";
    g.expression_level = 0.1;
    g.variants.push_back({"rs1", 0.9}); // High impact should pull expression up
    
    engine.add_gene(g);
    
    // Run for several steps
    for (int i = 0; i < 100; ++i) {
        engine.step(1.0);
    }
    
    auto updated_genes = engine.get_genes();
    ASSERT_TRUE(updated_genes.at("TEST").expression_level > 0.1);
}

TEST_CASE(SimulationEngine, PathwayActivation) {
    SimulationEngine engine;
    Gene g1{"G1", 0.2, {}};
    Gene g2{"G2", 0.2, {}};
    engine.add_gene(g1);
    engine.add_gene(g2);
    
    Pathway p{"Path1", {"G1", "G2"}, 0.5};
    engine.add_pathway(p);
    
    engine.step(0.0); // Update activation
    ASSERT_FALSE(engine.get_pathways()[0].is_active);
    
    // Manually force expression high
    Gene high_g1{"G1", 0.8, {}};
    Gene high_g2{"G2", 0.8, {}};
    engine.add_gene(high_g1);
    engine.add_gene(high_g2);
    
    engine.step(0.0);
    ASSERT_TRUE(engine.get_pathways()[0].is_active);
}
