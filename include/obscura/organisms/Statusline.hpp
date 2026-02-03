#pragma once
#include "obscura/ecology/Agent.hpp"
#include <string>

namespace obscura {

    // Minimal statusline agent: emits a single row of text at bottom.
    class Statusline : public Agent {
    public:
        std::string text = "Obscura";
        std::string prefix;
        std::string suffix;
        int priority = 5;
        int update_interval = 1;

        const char* name() const noexcept override { return "Statusline"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;

    private:
        int tick_counter_ = 0;
    };

} // namespace obscura
