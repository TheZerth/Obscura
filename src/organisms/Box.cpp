#include "obscura/organisms/Box.hpp"
#include "obscura/ecology/World.hpp"

namespace obscura {

    void Box::tick(World& world) {
        if (w < 2 || h < 2) return;

        auto emit_char = [&](int px, int py, char ch) {
            Claim c;
            c.x = px;
            c.y = py;
            c.glyph = std::string(1, ch);
            c.priority = priority;
            world.emit(std::move(c));
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
