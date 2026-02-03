#include "obscura/organisms/Menu.hpp"
#include <algorithm>

namespace obscura {

    ViewSpec Menu::view_spec(WorldSize) const {
        int width = 0;
        for (const auto& item : items) {
            width = std::max(width, static_cast<int>(item.size()));
        }
        int region_x = x - 2;
        int region_y = y;
        int region_w = width + 2;
        int region_h = static_cast<int>(items.size());
        if (region_w < 1) region_w = 1;
        if (region_h < 1) region_h = 1;
        int cx = region_x + region_w / 2;
        int cy = region_y + region_h / 2;
        int radius = std::max(1, std::max(region_w, region_h) / 2);
        return ViewSpec{cx, cy, radius};
    }

    void Menu::tick(const LocalView&, std::vector<Claim>& out_claims) {
        // Emits text claims for each item, one glyph per column (minimal ASCII).
        for (int i = 0; i < static_cast<int>(items.size()); ++i) {
            Claim indicator;
            indicator.x = x - 2;
            indicator.y = y + i;
            indicator.glyph = (i == selected) ? ">" : " ";
            indicator.priority = 20;
            if (indicator.x >= 0) out_claims.push_back(std::move(indicator));

            const auto& s = items[i];
            for (int j = 0; j < static_cast<int>(s.size()); ++j) {
                Claim c;
                c.x = x + j;
                c.y = y + i;
                c.glyph = std::string(1, s[static_cast<size_t>(j)]);
                c.priority = (i == selected) ? 10 : 1;
                out_claims.push_back(std::move(c));
            }
        }
    }

} // namespace obscura
