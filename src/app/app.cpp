#include "app.h"
#include "../api/api_handler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>

App::App() : cache(nullptr), state_manager(nullptr) {
    load_rules();
    
    // Initialize subsystems using rules
    auto& persistence = app_rules.object_value.at("persistence");
    cache = new CacheManager(persistence.object_value.at("cache_dir").string_value);
    state_manager = new StateManager(persistence.object_value.at("state_file").string_value);
    
    load_persistent_state();
    
    int interval = static_cast<int>(persistence.object_value.at("autosave_interval_ms").number_value);
    state_manager->auto_save_enable(interval, [this]() { return get_current_state(); });
    
    log_audit("Application initialized with rules: " + app_rules.serialize());
}

App::~App() {
    if (state_manager) {
        state_manager->auto_save_disable();
        delete state_manager;
    }
    delete cache;
    log_audit("Application terminated.");
}

void App::load_rules() {
    std::ifstream f("rules/app_rules.json");
    if (!f.is_open()) {
        std::cerr << "Critical Error: Could not load app_rules.json" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << f.rdbuf();
    app_rules = JsonValue::parse(buffer.str());
    
    // Populate command registry
    for (const auto& cmd : app_rules.object_value.at("commands").array_value) {
        CommandDef def{
            cmd.object_value.at("name").string_value,
            cmd.object_value.at("description").string_value,
            cmd.object_value.at("alias").string_value,
            cmd.object_value.at("requires_args").bool_value
        };
        command_registry[def.name] = def;
        if (!def.alias.empty()) {
            alias_map[def.alias] = def.name;
        }
    }
}

void App::log_audit(const std::string& message) {
    std::ofstream f("docs/runtime_audits.md", std::ios::app);
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    f << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] " << message << "\n";
}

void App::load_persistent_state() {
    JsonValue state = state_manager->load_state();
    if (state.type == JsonValue::OBJECT) {
        if (state.object_value.count("model")) current_cfg.model = state.object_value.at("model").string_value;
        if (state.object_value.count("temperature")) current_cfg.temperature = state.object_value.at("temperature").number_value;
    } else {
        auto& def = app_rules.object_value.at("default_config");
        current_cfg = {
            def.object_value.at("model").string_value,
            "",
            def.object_value.at("temperature").number_value,
            static_cast<int>(def.object_value.at("max_tokens").number_value),
            def.object_value.at("top_p").number_value,
            def.object_value.at("repeat_penalty").number_value
        };
    }
}

JsonValue App::get_current_state() {
    JsonValue state = JsonValue::makeObject();
    state.object_value["model"] = JsonValue::makeString(current_cfg.model);
    state.object_value["temperature"] = JsonValue::makeNumber(current_cfg.temperature);
    state.object_value["prompt"] = JsonValue::makeString(current_cfg.prompt);
    return state;
}

void App::run_cli(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: quanta_cogno <command> [args...]\n";
        return;
    }
    std::string cmd_name = argv[1];
    std::string args = (argc > 2) ? argv[2] : "";
    
    // If it's an alias, map it
    if (alias_map.count(cmd_name)) {
        cmd_name = alias_map[cmd_name];
    }
    
    process_command(cmd_name, args);
}

void App::run_tui() {
    log_audit("Entered TUI mode.");
    bool running = true;
    while (running) {
        display_menu();
        std::string line;
        std::getline(std::cin, line);
        
        if (line == "9" || line == "exit") {
            running = false;
        } else {
            // Check alias map first
            if (alias_map.count(line)) {
                std::string cmd_name = alias_map[line];
                auto def = command_registry.at(cmd_name);
                std::string args = "";
                if (def.requires_args) {
                    std::cout << "Enter arguments for " << cmd_name << ": ";
                    std::getline(std::cin, args);
                }
                process_command(cmd_name, args);
            } else {
                process_command(line, "");
            }
        }
    }
}

void App::display_menu() {
    std::cout << "\n--- " << app_rules.object_value.at("app_name").string_value << " v" << app_rules.object_value.at("version").string_value << " ---\n";
    for (const auto& cmd_pair : command_registry) {
        const auto& def = cmd_pair.second;
        std::cout << def.alias << ") " << def.name << " (" << def.description << ")\n";
    }
    std::cout << "> ";
}

void App::process_command(const std::string& cmd_name, const std::string& args) {
    log_audit("Processing command: " + cmd_name + " with args: " + args);
    if (cmd_name == "request") {
        cmd_request(args);
    } else if (cmd_name == "set-model") {
        cmd_set_model(args);
    } else if (cmd_name == "clear-cache") {
        cmd_clear_cache(args);
    } else {
        std::cout << "Unknown command: " << cmd_name << "\n";
        log_audit("Failed to process unknown command: " + cmd_name);
    }
}

void App::cmd_request(const std::string& args) {
    if (args.empty()) {
        std::cout << "Error: request requires an endpoint name.\n";
        return;
    }
    handle_request(args, JsonValue::makeObject());
}

void App::cmd_set_model(const std::string& args) {
    if (args.empty()) {
        std::cout << "Error: set-model requires a path.\n";
        return;
    }
    current_cfg.model = args;
    state_manager->update_state(get_current_state());
    std::cout << "Model updated to: " << args << "\n";
}

void App::cmd_clear_cache(const std::string&) {
    cache->clear();
    std::cout << "Cache cleared.\n";
}

void App::handle_request(const std::string& endpoint, const JsonValue& params) {
    std::string cache_key = endpoint + "_" + params.serialize();
    if (cache->has(cache_key)) {
        std::cout << "Returning cached data for " << endpoint << "\n";
        std::cout << cache->get(cache_key).serialize() << "\n";
        return;
    }

    std::cout << "Fetching data from API for " << endpoint << "...\n";
    JsonValue request = build_request(current_cfg);
    JsonValue response = simulate_api_call(request);
    
    cache->set(cache_key, response);
    std::cout << "Response: " << response.serialize() << "\n";
}
