#include "obscura/ecology/CellSettle.hpp"

namespace obscura {

    int CellSettle::choose(const std::vector<Claim>& claims) const {
        if (claims.empty()) return -1;
        int best_i = 0;
        for (int i = 1; i < static_cast<int>(claims.size()); ++i) {
            const Claim& c = claims[i];
            const Claim& best = claims[best_i];
            if (c.priority > best.priority) {
                best_i = i;
                continue;
            }
            if (c.priority < best.priority) continue;
            if (c.glyph < best.glyph) {
                best_i = i;
                continue;
            }
            if (c.glyph > best.glyph) continue;
            if (c.y < best.y) {
                best_i = i;
                continue;
            }
            if (c.y > best.y) continue;
            if (c.x < best.x) {
                best_i = i;
                continue;
            }
        }
        return best_i;
    }

} // namespace obscura
