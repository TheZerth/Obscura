#pragma once
#include "obscura/ecology/Agent.hpp"
#include <string>

namespace obscura {

    // Minimal statusline agent: emits a single row of text at bottom.
    class Statusline : public Agent {
    public:
        std::string text = "Obscura";
        int priority = 5;

        void tick(World& world) override;
    };

} // namespace obscura
