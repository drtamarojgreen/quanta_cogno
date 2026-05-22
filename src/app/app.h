#ifndef APP_H
#define APP_H

#include "../core/cache_manager.h"
#include "../core/state_manager.h"
#include "../api/api_logic.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

struct CommandDef {
    std::string name;
    std::string description;
    std::string alias;
    bool requires_args;
};

class App {
public:
    App();
    ~App();

    void run_cli(int argc, char* argv[]);
    void run_tui();

private:
    CacheManager* cache;
    StateManager* state_manager;
    ApiConfig current_cfg;
    JsonValue app_rules;
    std::map<std::string, CommandDef> command_registry;
    std::map<std::string, std::string> alias_map;

    void load_rules();
    void load_persistent_state();
    JsonValue get_current_state();
    
    void log_audit(const std::string& message);
    void handle_request(const std::string& endpoint, const JsonValue& params);
    void display_menu();
    void process_command(const std::string& cmd_name, const std::string& args);
    
    // Command Handlers
    void cmd_request(const std::string& args);
    void cmd_set_model(const std::string& args);
    void cmd_clear_cache(const std::string& args);
};

#endif // APP_H
