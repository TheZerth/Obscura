#pragma once
#include <cstdint>
#include <string>

namespace obscura {

    struct Claim {
        int x = 0;
        int y = 0;
        std::string glyph = " ";
        int priority = 0; // higher wins (for now)
    };

} // namespace obscura
