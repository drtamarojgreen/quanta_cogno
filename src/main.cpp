#include <iostream>
#include <string>
#include "api_logic.h"

void show_splash() {
    std::cout << "===========================================\n";
    std::cout << "   Cognitive Modeling Config Tool v1.0     \n";
    std::cout << "===========================================\n";
}

void show_menu() {
    std::cout << "\nConfiguration Menu:\n";
    std::cout << "1) Set model (current)\n";
    std::cout << "2) Set prompt (current)\n";
    std::cout << "3) Set temperature\n";
    std::cout << "4) Set max_tokens\n";
    std::cout << "5) Set top_p\n";
    std::cout << "6) Set repeat_penalty\n";
    std::cout << "7) Build & save request.json\n";
    std::cout << "8) Simulate API call & save response.json\n";
    std::cout << "9) Exit\n";
    std::cout << "Enter choice or type natural language command:\n> ";
}

int main() {
    ApiConfig cfg{
        "./models/llama-2-13b-chat.Q4_K_M.gguf",
        "",
        0.7,
        1024,
        0.9,
        1.1
    };

    show_splash();
    bool running = true;
    while(running) {
        show_menu();
        std::string input;
        std::getline(std::cin, input);
        if(input == "1") {
            std::cout << "Enter model path: "; std::getline(std::cin, cfg.model);
        } else if(input == "2") {
            std::cout << "Enter prompt: "; std::getline(std::cin, cfg.prompt);
        } else if(input == "3") {
            std::cout << "Enter temperature: "; std::cin >> cfg.temperature; std::cin.ignore();
        } else if(input == "4") {
            std::cout << "Enter max_tokens: "; std::cin >> cfg.max_tokens; std::cin.ignore();
        } else if(input == "5") {
            std::cout << "Enter top_p: "; std::cin >> cfg.top_p; std::cin.ignore();
        } else if(input == "6") {
            std::cout << "Enter repeat_penalty: "; std::cin >> cfg.repeat_penalty; std::cin.ignore();
        } else if(input == "7") {
            auto req = build_request(cfg);
            if(save_to_file("request.json", req))
                std::cout << "Saved request.json\n";
            else std::cout << "Error saving file.\n";
        } else if(input == "8") {
            auto req = build_request(cfg);
            auto resp = simulate_api_call(req);
            if(save_to_file("response.json", resp))
                std::cout << "Saved response.json\n";
            else std::cout << "Error saving file.\n";
        } else if(input == "9") {
            running = false;
        } else {
            // rudimentary natural language parser
            if(input.find("temperature") != std::string::npos) {
                std::cout << "Enter temperature: "; std::cin >> cfg.temperature; std::cin.ignore();
            } else if(input.find("prompt") != std::string::npos) {
                std::cout << "Enter prompt: "; std::getline(std::cin, cfg.prompt);
            } else {
                std::cout << "Unrecognized command. Please choose a menu number.\n";
            }
        }
    }
    std::cout << "Exiting. Goodbye!\n";
    return 0;
}
