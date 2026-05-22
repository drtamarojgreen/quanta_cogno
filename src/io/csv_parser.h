#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <vector>
#include <map>

namespace qc::io {

using CsvRow = std::vector<std::string>;
using CsvData = std::vector<CsvRow>;

class CsvParser {
public:
    static CsvData parse(const std::string& input, char delimiter = ',');
    static std::vector<std::map<std::string, std::string>> parse_to_maps(const std::string& input, char delimiter = ',');

private:
    static CsvRow parse_line(const std::string& line, char delimiter);
};

} // namespace qc::io

#endif // CSV_PARSER_H
