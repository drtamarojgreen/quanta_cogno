#include "app.h"
#include "../visualization/genomic_views.h"
#include <iostream>
#include <chrono>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace qc::app {

App::App() : vt(80, 24) {
    init_simulation();
    init_themes();
}

App::~App() {}

void App::init_simulation() {
    core::Gene htr2a{"HTR2A", 0.5, {{"rs6311", 0.8}}};
    core::Gene comt{"COMT", 0.3, {{"rs4680", 0.2}}};
    engine.add_gene(htr2a);
    engine.add_gene(comt);
}

void App::init_themes() {
    themes.push_back({"Classic", visualization::FG_WHITE, visualization::FG_CYAN});
    themes.push_back({"Matrix", visualization::FG_GREEN, visualization::FG_BLACK});
    themes.push_back({"Sunset", visualization::FG_MAGENTA, visualization::FG_YELLOW});
}

#ifndef _WIN32
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
char getch() { return getchar(); }
#endif

void App::run() {
    running = true;
    auto last_time = std::chrono::high_resolution_clock::now();
    
    // Clear screen initially
    std::cout << "\033[2J" << std::flush;

    while (running) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - last_time;
        last_time = now;

        process_input();
        if (!paused) {
            update(elapsed.count() * simulation_speed);
        }
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

void App::process_input() {
#ifdef _WIN32
    if (_kbhit()) {
        handle_keypress(_getch());
    }
#else
    if (kbhit()) {
        handle_keypress(getch());
    }
#endif
}

void App::handle_keypress(char c) {
    switch (c) {
        case 'q': running = false; break;
        case ' ': paused = !paused; break;
        case '+': simulation_speed += 0.1; break;
        case '-': simulation_speed -= 0.1; if (simulation_speed < 0) simulation_speed = 0; break;
        case 't': current_theme_idx = (current_theme_idx + 1) % themes.size(); break;
    }
}

void App::update(double dt) {
    engine.step(dt);
}

void App::render() {
    vt.clear();
    const auto& theme = themes[current_theme_idx];
    
    vt.set_cell(0, 0, "Quanta Cogno v2 - Genomic Simulation", theme.primary_color);
    vt.set_cell(0, 1, "Theme: " + theme.name + " | Speed: " + std::to_string(simulation_speed), theme.secondary_color);
    vt.set_cell(0, 2, paused ? "[PAUSED]" : "[RUNNING]", visualization::FG_YELLOW);

    auto genes_map = engine.get_genes();
    std::vector<core::Gene> genes_vec;
    int y = 4;
    for (const auto& [id, gene] : genes_map) {
        vt.set_cell(2, y, id + ": " + std::to_string(gene.expression_level), theme.primary_color);
        genes_vec.push_back(gene);
        y++;
    }

    visualization::GenomicViews::draw_heatmap(vt, 2, y + 1, genes_vec);

    std::cout << "\033[H" << vt.render() << std::flush;
}

} // namespace qc::app
