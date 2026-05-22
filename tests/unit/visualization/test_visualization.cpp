#include "visualization/virtual_terminal.h"
#include "visualization/genomic_views.h"
#include "utils/testing_framework.h"

using namespace qc::visualization;

TEST_CASE(VirtualTerminal, CanBufferCells) {
    VirtualTerminal vt(10, 10);
    vt.set_cell(0, 0, "X", FG_RED);
    std::string out = vt.render();
    
    ASSERT_TRUE(out.find('X') != std::string::npos);
    ASSERT_TRUE(out.find(FG_RED) != std::string::npos);
}

TEST_CASE(GenomicViews, DrawsColorizedSequence) {
    VirtualTerminal vt(10, 1);
    GenomicViews::draw_sequence(vt, 0, 0, "ACGT");
    std::string out = vt.render();
    
    // Check for character presence and color sequences
    ASSERT_TRUE(out.find('A') != std::string::npos);
    ASSERT_TRUE(out.find(FG_GREEN) != std::string::npos); // A is Green
    ASSERT_TRUE(out.find('T') != std::string::npos);
    ASSERT_TRUE(out.find(FG_RED) != std::string::npos);   // T is Red
}
