#include "csv_parser.h"
#include <sstream>

namespace qc::io {

CsvData CsvParser::parse(const std::string& input, char delimiter) {
    CsvData data;
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();
        data.push_back(parse_line(line, delimiter));
    }
    return data;
}

std::vector<std::map<std::string, std::string>> CsvParser::parse_to_maps(const std::string& input, char delimiter) {
    CsvData raw = parse(input, delimiter);
    if (raw.empty()) return {};
    
    const auto& headers = raw[0];
    std::vector<std::map<std::string, std::string>> result;
    for (size_t i = 1; i < raw.size(); ++i) {
        std::map<std::string, std::string> row_map;
        for (size_t j = 0; j < headers.size() && j < raw[i].size(); ++j) {
            row_map[headers[j]] = raw[i][j];
        }
        result.push_back(row_map);
    }
    return result;
}

CsvRow CsvParser::parse_line(const std::string& line, char delimiter) {
    CsvRow row;
    std::string field;
    bool in_quotes = false;
    
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (in_quotes && i + 1 < line.size() && line[i+1] == '"') {
                field += '"';
                i++;
            } else {
                in_quotes = !in_quotes;
            }
        } else if (c == delimiter && !in_quotes) {
            row.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    row.push_back(field);
    return row;
}

} // namespace qc::io
