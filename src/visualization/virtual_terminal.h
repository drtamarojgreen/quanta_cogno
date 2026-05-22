#ifndef VIRTUAL_TERMINAL_H
#define VIRTUAL_TERMINAL_H

#include <string>
#include <vector>

namespace qc::visualization {

struct Cell {
    std::string character = " ";
    std::string fg_color = ""; // ANSI color codes
    std::string bg_color = "";
};

class VirtualTerminal {
public:
    VirtualTerminal(int width, int height);
    
    void set_cell(int x, int y, const std::string& c, const std::string& fg = "", const std::string& bg = "");
    void clear();
    std::string render() const;

    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    int width, height;
    std::vector<Cell> buffer;
};

// ANSI Color Constants
const std::string FG_RED = "\033[31m";
const std::string FG_GREEN = "\033[32m";
const std::string FG_YELLOW = "\033[33m";
const std::string FG_BLUE = "\033[34m";
const std::string FG_MAGENTA = "\033[35m";
const std::string FG_CYAN = "\033[36m";
const std::string FG_WHITE = "\033[37m";
const std::string FG_BLACK = "\033[30m";
const std::string FG_RESET = "\033[39m";
const std::string RESET = "\033[0m";

} // namespace qc::visualization

#endif // VIRTUAL_TERMINAL_H
