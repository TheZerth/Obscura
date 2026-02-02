#include "obscura/term/Input.hpp"

namespace obscura {

    Input::Input(int fd_in, InputConfig cfg) : fd_(fd_in), cfg_(cfg) {}

    std::optional<Event> Input::read_event() {
        // Minimal stub: no input yet.
        return std::nullopt;
    }

} // namespace obscura
