#include "io/bmp_exporter.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(BmpExporter, ExportsCorrectHeader) {
    int w = 2, h = 2;
    std::vector<Color> pixels(w * h, {255, 0, 0}); // All red
    auto buffer = BmpExporter::export_to_buffer(w, h, pixels);
    
    ASSERT_TRUE(buffer.size() > 54);
    ASSERT_EQUAL(buffer[0], 'B');
    ASSERT_EQUAL(buffer[1], 'M');
    
    // Check width in info header (offset 18)
    uint32_t width = buffer[18] | (buffer[19] << 8) | (buffer[20] << 16) | (buffer[21] << 24);
    ASSERT_EQUAL(width, (uint32_t)w);
}
