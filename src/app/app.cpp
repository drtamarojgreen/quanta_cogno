#include "app.h"
#include "../api/api_handler.h"
#include <iostream>
#include <sstream>

App::App() : cache("./cache"), state_manager("app_state.json") {
    load_persistent_state();
    state_manager.auto_save_enable(5000, [this]() { return get_current_state(); });
}

App::~App() {
    state_manager.auto_save_disable();
}

void App::load_persistent_state() {
    JsonValue state = state_manager.load_state();
    if (state.type == JsonValue::OBJECT) {
        if (state.object_value.count("model")) current_cfg.model = state.object_value["model"].string_value;
        if (state.object_value.count("temperature")) current_cfg.temperature = state.object_value["temperature"].number_value;
        // ... load other fields
    } else {
        // Defaults
        current_cfg = { "./models/default.gguf", "", 0.7, 1024, 0.9, 1.1 };
    }
}

JsonValue App::get_current_state() {
    JsonValue state = JsonValue::makeObject();
    state.object_value["model"] = JsonValue::makeString(current_cfg.model);
    state.object_value["temperature"] = JsonValue::makeNumber(current_cfg.temperature);
    state.object_value["prompt"] = JsonValue::makeString(current_cfg.prompt);
    // ... save other fields
    return state;
}

void App::run_cli(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: quanta_cogno <command> [args...]\n";
        return;
    }
    std::string cmd = argv[1];
    if (cmd == "request") {
        if (argc < 3) {
            std::cout << "Usage: request <endpoint>\n";
            return;
        }
        handle_request(argv[2], JsonValue::makeObject());
    } else if (cmd == "clear-cache") {
        cache.clear();
        std::cout << "Cache cleared.\n";
    } else {
        std::cout << "Unknown command: " << cmd << "\n";
    }
}

void App::run_tui() {
    bool running = true;
    while (running) {
        display_menu();
        std::string line;
        std::getline(std::cin, line);
        if (line == "9" || line == "exit") {
            running = false;
        } else {
            process_tui_command(line);
        }
    }
}

void App::display_menu() {
    std::cout << "\n--- Quanta Cogno TUI ---\n";
    std::cout << "1) Set Model (" << current_cfg.model << ")\n";
    std::cout << "2) Set Prompt\n";
    std::cout << "3) Request Data\n";
    std::cout << "4) View Cache\n";
    std::cout << "5) Clear Cache\n";
    std::cout << "9) Exit\n";
    std::cout << "> ";
}

void App::process_tui_command(const std::string& cmd) {
    if (cmd == "1") {
        std::cout << "Enter model path: ";
        std::getline(std::cin, current_cfg.model);
        state_manager.update_state(get_current_state());
    } else if (cmd == "3") {
        std::cout << "Enter endpoint: ";
        std::string ep;
        std::getline(std::cin, ep);
        handle_request(ep, JsonValue::makeObject());
    } else if (cmd == "5") {
        cache.clear();
        std::cout << "Cache cleared.\n";
    }
}

void App::handle_request(const std::string& endpoint, const JsonValue& params) {
    std::string cache_key = endpoint + "_" + params.serialize();
    if (cache.has(cache_key)) {
        std::cout << "Returning cached data for " << endpoint << "\n";
        std::cout << cache.get(cache_key).serialize() << "\n";
        return;
    }

    std::cout << "Fetching data from API for " << endpoint << "...\n";
    JsonValue request = build_request(current_cfg);
    // Merge params into request if needed
    JsonValue response = simulate_api_call(request);
    
    cache.set(cache_key, response);
    std::cout << "Response: " << response.serialize() << "\n";
}
