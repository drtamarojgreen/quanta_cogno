#ifndef GENOMIC_VIEWS_H
#define GENOMIC_VIEWS_H

#include "virtual_terminal.h"
#include "../core/genomic_primitives.h"
#include <vector>

namespace qc::visualization {

class GenomicViews {
public:
    static void draw_sequence(VirtualTerminal& vt, int x, int y, const std::string& sequence);
    static void draw_heatmap(VirtualTerminal& vt, int x, int y, const std::vector<core::Gene>& genes);
};

} // namespace qc::visualization

#endif // GENOMIC_VIEWS_H
