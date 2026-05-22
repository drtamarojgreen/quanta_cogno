#ifndef STATE_MANAGER_V2_H
#define STATE_MANAGER_V2_H

#include "../io/json_parser.h"
#include <string>
#include <functional>
#include <atomic>
#include <thread>

#include <mutex>

namespace qc::core {

class StateManager {
public:
    StateManager(const std::string& state_file = "app_state.json");
    ~StateManager();

    void start_autosave(int interval_ms, std::function<io::JsonValue()> state_provider);
    void stop_autosave();
    
    void save_immediate(const io::JsonValue& state);
    std::optional<io::JsonValue> load();

private:
    std::string state_file;
    std::atomic<bool> autosave_running{false};
    std::thread autosave_thread;
    mutable std::mutex state_mutex;
};

} // namespace qc::core

#endif // STATE_MANAGER_V2_H
