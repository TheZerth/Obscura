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

        void tick(World& world) override;
    };

} // namespace obscura
