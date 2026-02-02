#include "obscura/term/Terminal.hpp"

int main() {
    auto term = obscura::Terminal::open();
    term.write("Obscura demo_rules_playground (bootstrap)\r\n");
    term.flush();
    return 0;
}
