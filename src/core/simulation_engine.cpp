#include "simulation_engine.h"
#include <cmath>
#include <numeric>

namespace qc::core {

void SimulationEngine::add_gene(const Gene& gene) {
    genes[gene.id] = gene;
}

void SimulationEngine::add_pathway(const Pathway& pathway) {
    pathways.push_back(pathway);
}

void SimulationEngine::step(double dt) {
    for (auto& [id, gene] : genes) {
        update_expression(gene, dt);
    }
    update_pathways();
}

void SimulationEngine::update_expression(Gene& gene, double dt) {
    // Basic expression model: drift based on variant impact
    double total_impact = 0.0;
    for (const auto& v : gene.variants) {
        total_impact += v.impact;
    }
    
    // Normalize impact and apply a sigmoid-like drift
    double target = total_impact > 0 ? (total_impact / gene.variants.size()) : 0.5;
    double diff = target - gene.expression_level;
    
    // Use a simple linear interpolation for the simulation step
    gene.expression_level += diff * dt * 0.1;
    
    // Clamp
    if (gene.expression_level < 0.0) gene.expression_level = 0.0;
    if (gene.expression_level > 1.0) gene.expression_level = 1.0;
}

void SimulationEngine::update_pathways() {
    for (auto& pathway : pathways) {
        double avg_expression = 0.0;
        int count = 0;
        for (const auto& gene_id : pathway.gene_ids) {
            if (genes.count(gene_id)) {
                avg_expression += genes[gene_id].expression_level;
                count++;
            }
        }
        
        if (count > 0) {
            avg_expression /= count;
            pathway.is_active = (avg_expression >= pathway.activation_threshold);
        }
    }
}

} // namespace qc::core
