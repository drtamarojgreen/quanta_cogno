#include "state_manager.h"
#include <fstream>
#include <iostream>

namespace qc::core {

StateManager::StateManager(const std::string& state_file) : state_file(state_file) {}

StateManager::~StateManager() {
    stop_autosave();
}

void StateManager::save_immediate(const io::JsonValue& state) {
    std::lock_guard<std::mutex> lock(state_mutex);
    std::ofstream f(state_file);
    f << "{ \"state\": \"saved\" }"; // Placeholder serialization
}

std::optional<io::JsonValue> StateManager::load() {
    std::lock_guard<std::mutex> lock(state_mutex);
    std::ifstream f(state_file);
    if (!f.is_open()) return std::nullopt;
    
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    auto res = io::JsonParser::parse(content);
    if (std::holds_alternative<io::JsonValue>(res)) {
        return std::get<io::JsonValue>(res);
    }
    return std::nullopt;
}

void StateManager::start_autosave(int interval_ms, std::function<io::JsonValue()> state_provider) {
    if (autosave_running) return;
    autosave_running = true;
    autosave_thread = std::thread([this, interval_ms, state_provider]() {
        while (autosave_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            if (autosave_running) {
                save_immediate(state_provider());
            }
        }
    });
}

void StateManager::stop_autosave() {
    autosave_running = false;
    if (autosave_thread.joinable()) {
        autosave_thread.join();
    }
}

} // namespace qc::core
