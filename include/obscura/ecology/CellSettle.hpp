#pragma once
#include "obscura/ecology/Claim.hpp"
#include <vector>

namespace obscura {

    // Minimal cell settle: pick the highest-priority claim.
    class CellSettle {
    public:
        int choose(const std::vector<Claim>& claims) const; // index or -1
    };

} // namespace obscura
