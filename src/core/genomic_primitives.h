#ifndef GENOMIC_PRIMITIVES_H
#define GENOMIC_PRIMITIVES_H

#include <string>
#include <vector>
#include <map>

namespace qc::core {

struct Variant {
    std::string id;
    double impact; // 0.0 to 1.0
};

struct Gene {
    std::string id;
    double expression_level; // 0.0 to 1.0
    std::vector<Variant> variants;
};

struct Pathway {
    std::string name;
    std::vector<std::string> gene_ids;
    double activation_threshold;
    bool is_active = false;
};

} // namespace qc::core

#endif // GENOMIC_PRIMITIVES_H
