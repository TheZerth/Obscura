#include "obscura/organisms/Menu.hpp"
#include <algorithm>

namespace obscura {

    ViewSpec Menu::view_spec(WorldSize) const {
        int width = 0;
        for (const auto& item : items) {
            width = std::max(width, static_cast<int>(item.size()));
        }
        int region_x = x + padding_x - 2;
        int region_y = y + padding_y;
        int region_w = width + 2 + padding_x;
        int region_h = static_cast<int>(items.size()) + padding_y;
        if (region_w < 1) region_w = 1;
        if (region_h < 1) region_h = 1;
        int cx = region_x + region_w / 2;
        int cy = region_y + region_h / 2;
        int radius = std::max(1, std::max(region_w, region_h) / 2);
        return ViewSpec{cx, cy, radius};
    }

    void Menu::tick(const LocalView&, std::vector<Claim>& out_claims) {
        tick_counter_++;
        if (items.empty()) return;

        if (visible_selected_ < 0 || visible_selected_ >= static_cast<int>(items.size())) {
            visible_selected_ = std::max(0, std::min(selected, static_cast<int>(items.size()) - 1));
        }

        if (selected != visible_selected_) {
            if (scroll_delay_ticks <= 0) {
                visible_selected_ = selected;
                scroll_delay_counter_ = 0;
            } else {
                scroll_delay_counter_++;
                if (scroll_delay_counter_ >= scroll_delay_ticks) {
                    visible_selected_ = selected;
                    scroll_delay_counter_ = 0;
                }
            }
        } else {
            scroll_delay_counter_ = 0;
        }

        if (update_interval > 1 && (tick_counter_ % update_interval) != 0) return;

        int base_x = x + padding_x;
        int base_y = y + padding_y;

        // Emits text claims for each item, one glyph per column (minimal ASCII).
        for (int i = 0; i < static_cast<int>(items.size()); ++i) {
            Claim indicator;
            indicator.x = base_x - 2;
            indicator.y = base_y + i;
            indicator.glyph = (i == visible_selected_) ? highlight_glyph : " ";
            indicator.priority = 20;
            if (indicator.x >= 0) out_claims.push_back(std::move(indicator));

            const auto& s = items[i];
            for (int j = 0; j < static_cast<int>(s.size()); ++j) {
                Claim c;
                c.x = base_x + j;
                c.y = base_y + i;
                c.glyph = std::string(1, s[static_cast<size_t>(j)]);
                c.priority = (i == visible_selected_) ? 10 : 1;
                out_claims.push_back(std::move(c));
            }
        }
    }

} // namespace obscura
