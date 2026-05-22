#include "io/midi_exporter.h"
#include "utils/testing_framework.h"

using namespace qc::io;

TEST_CASE(MidiExporter, ExportsCorrectHeader) {
    std::vector<MidiNote> notes = {{60, 100, 0, 480}};
    auto buffer = MidiExporter::export_to_buffer(notes);
    
    ASSERT_TRUE(buffer.size() > 14);
    ASSERT_EQUAL(buffer[0], 'M');
    ASSERT_EQUAL(buffer[1], 'T');
    ASSERT_EQUAL(buffer[2], 'h');
    ASSERT_EQUAL(buffer[3], 'd');
}
