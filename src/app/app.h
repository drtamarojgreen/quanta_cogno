#ifndef APP_H
#define APP_H

#include "../core/simulation_engine.h"
#include "../visualization/virtual_terminal.h"
#include <string>
#include <atomic>
#include <thread>

namespace qc::app {

struct Theme {
    std::string name;
    std::string primary_color;
    std::string secondary_color;
};

class App {
public:
    App();
    ~App();

    void run();
    void stop() { running = false; }

private:
    core::SimulationEngine engine;
    visualization::VirtualTerminal vt;
    std::atomic<bool> running{false};
    double simulation_speed = 1.0;
    bool paused = false;
    
    std::vector<Theme> themes;
    size_t current_theme_idx = 0;

    void init_simulation();
    void init_themes();
    
    void process_input();
    void update(double dt);
    void render();
    
    void handle_keypress(char c);
};

} // namespace qc::app

#endif // APP_H
