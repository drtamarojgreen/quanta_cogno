#include "virtual_terminal.h"
#include <sstream>

namespace qc::visualization {

VirtualTerminal::VirtualTerminal(int width, int height) : width(width), height(height) {
    buffer.resize(width * height);
}

void VirtualTerminal::set_cell(int x, int y, const std::string& c, const std::string& fg, const std::string& bg) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = {c, fg, bg};
    }
}

void VirtualTerminal::clear() {
    for (auto& cell : buffer) {
        cell = {" ", "", ""};
    }
}

std::string VirtualTerminal::render() const {
    std::stringstream ss;
    std::string current_fg = "";
    std::string current_bg = "";

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& cell = buffer[y * width + x];
            
            if (cell.fg_color != current_fg || cell.bg_color != current_bg) {
                ss << RESET;
                if (!cell.fg_color.empty()) ss << cell.fg_color;
                if (!cell.bg_color.empty()) ss << cell.bg_color;
                current_fg = cell.fg_color;
                current_bg = cell.bg_color;
            }
            ss << cell.character;
        }
        ss << "\n";
    }
    ss << RESET;
    return ss.str();
}

} // namespace qc::visualization
