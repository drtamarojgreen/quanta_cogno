#include "genomic_views.h"
#include <algorithm>

namespace qc::visualization {

void GenomicViews::draw_sequence(VirtualTerminal& vt, int x, int y, const std::string& sequence) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        char c = sequence[i];
        std::string s(1, c);
        std::string color = FG_WHITE;
        switch (c) {
            case 'A': color = FG_GREEN; break;
            case 'C': color = FG_BLUE; break;
            case 'G': color = FG_YELLOW; break;
            case 'T': color = FG_RED; break;
        }
        vt.set_cell(x + i, y, s, color);
    }
}

void GenomicViews::draw_heatmap(VirtualTerminal& vt, int x, int y, const std::vector<core::Gene>& genes) {
    for (size_t i = 0; i < genes.size(); ++i) {
        double expr = genes[i].expression_level;
        std::string block = " ";
        std::string color = FG_RESET;
        
        if (expr > 0.8) { block = "█"; color = FG_RED; }
        else if (expr > 0.5) { block = "▓"; color = FG_YELLOW; }
        else if (expr > 0.2) { block = "▒"; color = FG_GREEN; }
        else { block = "░"; color = FG_BLUE; }
        
        vt.set_cell(x + i, y, block, color);
    }
}

} // namespace qc::visualization
