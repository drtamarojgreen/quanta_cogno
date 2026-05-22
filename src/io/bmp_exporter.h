#ifndef BMP_EXPORTER_H
#define BMP_EXPORTER_H

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

namespace qc::io {

struct Color {
    uint8_t r, g, b;
};

class BmpExporter {
public:
    static std::vector<uint8_t> export_to_buffer(int width, int height, const std::vector<Color>& pixels) {
        std::vector<uint8_t> buffer;
        
        uint32_t padding = (4 - (width * 3) % 4) % 4;
        uint32_t file_size = 54 + (width * 3 + padding) * height;
        
        // File Header
        buffer.push_back('B'); buffer.push_back('M');
        write_u32(buffer, file_size);
        write_u32(buffer, 0); // Reserved
        write_u32(buffer, 54); // Offset
        
        // Info Header
        write_u32(buffer, 40); // Size
        write_u32(buffer, width);
        write_u32(buffer, height);
        write_u16(buffer, 1); // Planes
        write_u16(buffer, 24); // Bits per pixel
        write_u32(buffer, 0); // Compression
        write_u32(buffer, (width * 3 + padding) * height); // Image size
        write_u32(buffer, 2835); // X ppm
        write_u32(buffer, 2835); // Y ppm
        write_u32(buffer, 0); // Colors
        write_u32(buffer, 0); // Important colors
        
        // Pixel Data (Bottom-up)
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                const auto& p = pixels[y * width + x];
                buffer.push_back(p.b);
                buffer.push_back(p.g);
                buffer.push_back(p.r);
            }
            for (uint32_t p = 0; p < padding; ++p) buffer.push_back(0);
        }
        
        return buffer;
    }

    static bool export_to_file(const std::string& filename, int width, int height, const std::vector<Color>& pixels) {
        auto buffer = export_to_buffer(width, height, pixels);
        std::ofstream f(filename, std::ios::binary);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        return true;
    }

private:
    static void write_u32(std::vector<uint8_t>& b, uint32_t v) {
        b.push_back(static_cast<uint8_t>(v & 0xFF));
        b.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
        b.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
        b.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
    }
    static void write_u16(std::vector<uint8_t>& b, uint16_t v) {
        b.push_back(static_cast<uint8_t>(v & 0xFF));
        b.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    }
};

} // namespace qc::io

#endif // BMP_EXPORTER_H
