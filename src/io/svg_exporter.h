#ifndef SVG_EXPORTER_H
#define SVG_EXPORTER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

namespace qc::io {

struct SvgPoint { double x, y; };
struct SvgCircle { SvgPoint center; double radius; std::string color; };
struct SvgLine { SvgPoint p1, p2; std::string color; double width; };

class SvgExporter {
public:
    static std::string export_to_string(int width, int height, const std::vector<SvgCircle>& circles, const std::vector<SvgLine>& lines) {
        std::stringstream ss;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        ss << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        
        // Draw lines
        for (const auto& l : lines) {
            ss << "  <line x1=\"" << l.p1.x << "\" y1=\"" << l.p1.y 
               << "\" x2=\"" << l.p2.x << "\" y2=\"" << l.p2.y 
               << "\" stroke=\"" << l.color << "\" stroke-width=\"" << l.width << "\" />\n";
        }
        
        // Draw circles
        for (const auto& c : circles) {
            ss << "  <circle cx=\"" << c.center.x << "\" cy=\"" << c.center.y 
               << "\" r=\"" << c.radius << "\" fill=\"" << c.color << "\" />\n";
        }
        
        ss << "</svg>";
        return ss.str();
    }

    static bool export_to_file(const std::string& filename, int width, int height, const std::vector<SvgCircle>& circles, const std::vector<SvgLine>& lines) {
        std::ofstream f(filename);
        if (!f) return false;
        f << export_to_string(width, height, circles, lines);
        return true;
    }
};

} // namespace qc::io

#endif // SVG_EXPORTER_H
