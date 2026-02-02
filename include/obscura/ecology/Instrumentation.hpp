#pragma once
#include <cstdint>

namespace obscura {

    struct Instrumentation {
        std::uint64_t ticks = 0;
        std::uint64_t claims_emitted = 0;
        std::uint64_t cells_written = 0;
        std::uint64_t contention_cells = 0;

        void reset_frame() {
            claims_emitted = 0;
            cells_written = 0;
            contention_cells = 0;
        }
    };

} // namespace obscura
