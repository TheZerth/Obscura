#include "obscura/ecology/CellSettle.hpp"

namespace obscura {

    int CellSettle::choose(const std::vector<Claim>& claims) const {
        if (claims.empty()) return -1;
        int best_i = 0;
        for (int i = 1; i < static_cast<int>(claims.size()); ++i) {
            if (claims[i].priority > claims[best_i].priority) best_i = i;
        }
        return best_i;
    }

} // namespace obscura