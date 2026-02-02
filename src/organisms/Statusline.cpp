#include "obscura/organisms/Statusline.hpp"
#include "obscura/ecology/World.hpp"

namespace obscura {

    void Statusline::tick(World& world) {
        int row = world.screen().rows() - 1;
        if (row < 0) return;

        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            Claim c;
            c.x = i;
            c.y = row;
            c.glyph = std::string(1, text[static_cast<size_t>(i)]);
            c.priority = priority;
            world.emit(std::move(c));
        }
    }

} // namespace obscura
