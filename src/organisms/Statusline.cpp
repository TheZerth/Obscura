#include "obscura/organisms/Statusline.hpp"
#include <algorithm>

namespace obscura {

    ViewSpec Statusline::view_spec(WorldSize world) const {
        int row = world.rows - 1;
        int width = static_cast<int>(text.size());
        if (width < 1) width = 1;
        int cx = width / 2;
        int radius = std::max(1, width / 2);
        return ViewSpec{cx, row, radius};
    }

    void Statusline::tick(const LocalView& view, std::vector<Claim>& out_claims) {
        int row = view.center_y();
        if (row < 0) return;

        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            Claim c;
            c.x = i;
            c.y = row;
            c.glyph = std::string(1, text[static_cast<size_t>(i)]);
            c.priority = priority;
            out_claims.push_back(std::move(c));
        }
    }

} // namespace obscura
