#ifndef MIDI_EXPORTER_H
#define MIDI_EXPORTER_H

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

namespace qc::io {

struct MidiNote {
    uint8_t pitch;
    uint8_t velocity;
    uint32_t start_tick;
    uint32_t duration_ticks;
};

class MidiExporter {
public:
    static std::vector<uint8_t> export_to_buffer(const std::vector<MidiNote>& notes) {
        std::vector<uint8_t> buffer;
        
        // MThd Header
        buffer.push_back('M'); buffer.push_back('T'); buffer.push_back('h'); buffer.push_back('d');
        write_u32_be(buffer, 6); // Header size
        write_u16_be(buffer, 0); // Format 0 (single track)
        write_u16_be(buffer, 1); // Number of tracks
        write_u16_be(buffer, 480); // Division (ticks per quarter note)
        
        // MTrk Track Chunk
        std::vector<uint8_t> track_data;
        uint32_t current_tick = 0;
        
        // Very simplified: Sort notes by start time and emit
        // In a real implementation, we'd need a delta-time event list
        for (const auto& n : notes) {
            uint32_t delta = n.start_tick - current_tick;
            write_varlen(track_data, delta);
            track_data.push_back(0x90); // Note On (Channel 1)
            track_data.push_back(n.pitch);
            track_data.push_back(n.velocity);
            
            write_varlen(track_data, n.duration_ticks);
            track_data.push_back(0x80); // Note Off
            track_data.push_back(n.pitch);
            track_data.push_back(0); // Velocity
            
            current_tick = n.start_tick + n.duration_ticks;
        }
        
        // End of track
        write_varlen(track_data, 0);
        track_data.push_back(0xFF); track_data.push_back(0x2F); track_data.push_back(0x00);
        
        buffer.push_back('M'); buffer.push_back('T'); buffer.push_back('r'); buffer.push_back('k');
        write_u32_be(buffer, track_data.size());
        buffer.insert(buffer.end(), track_data.begin(), track_data.end());
        
        return buffer;
    }

    static bool export_to_file(const std::string& filename, const std::vector<MidiNote>& notes) {
        auto buffer = export_to_buffer(notes);
        std::ofstream f(filename, std::ios::binary);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        return true;
    }

private:
    static void write_u32_be(std::vector<uint8_t>& b, uint32_t v) {
        b.push_back((v >> 24) & 0xFF);
        b.push_back((v >> 16) & 0xFF);
        b.push_back((v >> 8) & 0xFF);
        b.push_back(v & 0xFF);
    }
    static void write_u16_be(std::vector<uint8_t>& b, uint16_t v) {
        b.push_back((v >> 8) & 0xFF);
        b.push_back(v & 0xFF);
    }
    static void write_varlen(std::vector<uint8_t>& b, uint32_t v) {
        uint32_t buffer = v & 0x7F;
        while ((v >>= 7) > 0) {
            buffer <<= 8;
            buffer |= 0x80;
            buffer |= (v & 0x7F);
        }
        while (true) {
            b.push_back(buffer & 0xFF);
            if (buffer & 0x80) buffer >>= 8; else break;
        }
    }
};

} // namespace qc::io

#endif // MIDI_EXPORTER_H
