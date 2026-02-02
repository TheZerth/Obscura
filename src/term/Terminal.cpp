#include "obscura/term/Terminal.hpp"
#include <iostream>

namespace obscura {

    struct Terminal::Impl {};

    Terminal Terminal::open() { return Terminal{}; }
    Terminal::Terminal(Terminal&&) noexcept = default;
    Terminal& Terminal::operator=(Terminal&&) noexcept = default;
    Terminal::~Terminal() = default;

    void Terminal::write(std::string_view bytes) { std::cout << bytes; }
    void Terminal::flush() { std::cout << std::flush; }

} // namespace obscura