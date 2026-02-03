#pragma once
#include "obscura/ecology/Agent.hpp"
#include <string>

namespace obscura {

    // Minimal box agent: emits ASCII borders.
    class Box : public Agent {
    public:
        int x = 1;
        int y = 1;
        int w = 20;
        int h = 8;
        int priority = 0;
        int update_interval = 1;

        char corner_tl = '+';
        char corner_tr = '+';
        char corner_bl = '+';
        char corner_br = '+';
        char horiz = '-';
        char vert = '|';
        std::string fill_pattern;

        const char* name() const noexcept override { return "Box"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;

    private:
        int tick_counter_ = 0;
    };

} // namespace obscura
