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

        auto emit_char = [&](int px, int py, char ch) {
            Claim c;
            c.x = px;
            c.y = py;
            c.glyph = std::string(1, ch);
            c.priority = priority;
            out_claims.push_back(std::move(c));
        };

        // Corners
        emit_char(x,         y,         '+');
        emit_char(x + w - 1, y,         '+');
        emit_char(x,         y + h - 1, '+');
        emit_char(x + w - 1, y + h - 1, '+');

        // Horizontal
        for (int i = 1; i < w - 1; ++i) {
            emit_char(x + i, y,         '-');
            emit_char(x + i, y + h - 1, '-');
        }

        // Vertical
        for (int j = 1; j < h - 1; ++j) {
            emit_char(x,         y + j, '|');
            emit_char(x + w - 1, y + j, '|');
        }
    }

} // namespace obscura
