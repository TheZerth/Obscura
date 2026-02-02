#include "obscura/term/Terminal.hpp"

int main() {
    auto term = obscura::Terminal::open();
    term.write("Obscura demo_heatmap (bootstrap)\r\n");
    term.flush();
    return 0;
}
