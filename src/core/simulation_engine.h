#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include "genomic_primitives.h"
#include <vector>
#include <map>

namespace qc::core {

class SimulationEngine {
public:
    void add_gene(const Gene& gene);
    void add_pathway(const Pathway& pathway);
    
    void step(double dt);
    
    const std::map<std::string, Gene>& get_genes() const { return genes; }
    const std::vector<Pathway>& get_pathways() const { return pathways; }

private:
    std::map<std::string, Gene> genes;
    std::vector<Pathway> pathways;
    
    void update_expression(Gene& gene, double dt);
    void update_pathways();
};

} // namespace qc::core

#endif // SIMULATION_ENGINE_H
