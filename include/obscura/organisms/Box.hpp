#pragma once
#include "obscura/ecology/Agent.hpp"

namespace obscura {

    // Minimal box agent: emits ASCII borders.
    class Box : public Agent {
    public:
        int x = 1;
        int y = 1;
        int w = 20;
        int h = 8;
        int priority = 0;

        const char* name() const noexcept override { return "Box"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;
    };

} // namespace obscura
