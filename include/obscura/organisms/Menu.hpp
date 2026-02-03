#pragma once
#include "obscura/ecology/Agent.hpp"
#include <string>
#include <vector>

namespace obscura {

    // Minimal menu agent: emits claims for a list of strings.
    class Menu : public Agent {
    public:
        std::vector<std::string> items;
        int x = 2;
        int y = 2;
        int selected = 0;

        const char* name() const noexcept override { return "Menu"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;
    };

} // namespace obscura
