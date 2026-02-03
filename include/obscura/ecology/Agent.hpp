#pragma once
#include "obscura/ecology/Claim.hpp"
#include "obscura/ecology/LocalView.hpp"
#include <vector>

namespace obscura {

    struct WorldSize {
        int cols = 0;
        int rows = 0;
    };

    struct ViewSpec {
        int center_x = 0;
        int center_y = 0;
        int radius = 1;
    };

    struct Agent {
        virtual ~Agent() = default;
        virtual ViewSpec view_spec(WorldSize world) const = 0;
        virtual void tick(const LocalView& view, std::vector<Claim>& out_claims) = 0;
    };

} // namespace obscura
