#include "app/app.h"
#include <iostream>

int main(int argc, char* argv[]) {
    App app;
    if (argc > 1) {
        app.run_cli(argc, argv);
    } else {
        app.run_tui();
    }
    return 0;
}
