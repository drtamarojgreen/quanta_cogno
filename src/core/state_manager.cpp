#include "state_manager.h"
#include "../api/api_logic.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>

StateManager::StateManager(const std::string& state_file) : state_file(state_file) {}

void StateManager::update_state(const JsonValue& new_state) {
    save_state(new_state);
}

JsonValue StateManager::load_state() const {
    return load_from_file(state_file);
}

void StateManager::save_state(const JsonValue& state) {
    save_to_file(state_file, state);
}

// Simple auto-save implementation using a background thread
static std::atomic<bool> auto_save_running{false};
static std::thread auto_save_thread;

void StateManager::auto_save_enable(int interval_ms, std::function<JsonValue()> state_provider) {
    if (auto_save_running) return;
    auto_save_running = true;
    auto_save_thread = std::thread([this, interval_ms, state_provider]() {
        while (auto_save_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            if (auto_save_running) {
                save_state(state_provider());
            }
        }
    });
}

void StateManager::auto_save_disable() {
    auto_save_running = false;
    if (auto_save_thread.joinable()) {
        auto_save_thread.join();
    }
}
