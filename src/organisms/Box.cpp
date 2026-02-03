#include "obscura/organisms/Box.hpp"
#include <algorithm>

namespace obscura {

    ViewSpec Box::view_spec(WorldSize) const {
        int cx = x + w / 2;
        int cy = y + h / 2;
        int radius = std::max(1, std::max(w, h) / 2);
        return ViewSpec{cx, cy, radius};
    }

    void Box::tick(const LocalView&, std::vector<Claim>& out_claims) {
        if (w < 2 || h < 2) return;
        tick_counter_++;
        if (update_interval > 1 && (tick_counter_ % update_interval) != 0) return;

        auto emit_char = [&](int px, int py, char ch) {
            Claim c;
            c.x = px;
            c.y = py;
            c.glyph = std::string(1, ch);
            c.priority = priority;
            out_claims.push_back(std::move(c));
        };

        // Corners
        emit_char(x,         y,         corner_tl);
        emit_char(x + w - 1, y,         corner_tr);
        emit_char(x,         y + h - 1, corner_bl);
        emit_char(x + w - 1, y + h - 1, corner_br);

        // Horizontal
        for (int i = 1; i < w - 1; ++i) {
            emit_char(x + i, y,         horiz);
            emit_char(x + i, y + h - 1, horiz);
        }

        // Vertical
        for (int j = 1; j < h - 1; ++j) {
            emit_char(x,         y + j, vert);
            emit_char(x + w - 1, y + j, vert);
        }

        if (!fill_pattern.empty()) {
            for (int j = 1; j < h - 1; ++j) {
                for (int i = 1; i < w - 1; ++i) {
                    std::size_t idx = static_cast<std::size_t>((i + j) % fill_pattern.size());
                    emit_char(x + i, y + j, fill_pattern[idx]);
                }
            }
        }
    }

} // namespace obscura
