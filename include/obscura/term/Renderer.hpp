#pragma once
#include "obscura/term/Screen.hpp"

namespace obscura {

    class Terminal; // forward-declared to avoid include cycles

    struct RenderConfig {
        bool full_redraw = true; // minimal mode: redraw everything always
    };

    class Renderer {
    public:
        Renderer() = default;
        explicit Renderer(RenderConfig cfg) : cfg_(cfg) {}

        void draw(Terminal& term, const Screen& screen);
        void invalidate() { valid_ = false; }

    private:
        RenderConfig cfg_{};
        bool valid_ = false;
    };

} // namespace obscura
