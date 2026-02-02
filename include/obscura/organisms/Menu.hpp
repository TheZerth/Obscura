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

        void tick(World& world) override;
    };

} // namespace obscura
