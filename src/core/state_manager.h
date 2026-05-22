#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "json_logic.h"
#include <string>
#include <functional>

class StateManager {
public:
    StateManager(const std::string& state_file = "app_state.json");
    
    void update_state(const JsonValue& new_state);
    JsonValue load_state() const;
    void auto_save_enable(int interval_ms, std::function<JsonValue()> state_provider);
    void auto_save_disable();

private:
    std::string state_file;
    void save_state(const JsonValue& state);
};

#endif // STATE_MANAGER_H
