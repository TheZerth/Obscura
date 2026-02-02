#include "obscura/organisms/Menu.hpp"
#include "obscura/ecology/World.hpp"

namespace obscura {

    void Menu::tick(World& world) {
        // Emits text claims for each item, one glyph per column (minimal ASCII).
        for (int i = 0; i < static_cast<int>(items.size()); ++i) {
            const auto& s = items[i];
            for (int j = 0; j < static_cast<int>(s.size()); ++j) {
                Claim c;
                c.x = x + j;
                c.y = y + i;
                c.glyph = std::string(1, s[static_cast<size_t>(j)]);
                c.priority = (i == selected) ? 10 : 1;
                world.emit(std::move(c));
            }
        }
    }

} // namespace obscura
