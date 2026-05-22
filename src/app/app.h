#ifndef APP_H
#define APP_H

#include "../core/cache_manager.h"
#include "../core/state_manager.h"
#include "../api/api_logic.h"
#include <string>
#include <vector>

class App {
public:
    App();
    ~App();

    void run_cli(int argc, char* argv[]);
    void run_tui();

private:
    CacheManager cache;
    StateManager state_manager;
    ApiConfig current_cfg;

    void load_persistent_state();
    JsonValue get_current_state();
    
    void handle_request(const std::string& endpoint, const JsonValue& params);
    void display_menu();
    void process_tui_command(const std::string& cmd);
};

#endif // APP_H
