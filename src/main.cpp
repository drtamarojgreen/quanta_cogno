#include "app/app.h"

namespace qc {
    int run_app() {
        qc::app::App app;
        app.run();
        return 0;
    }
}

int main() {
    return qc::run_app();
}
