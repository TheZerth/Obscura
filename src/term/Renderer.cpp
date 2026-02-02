#include "obscura/term/Renderer.hpp"
#include "obscura/term/Terminal.hpp"
#include <string>

namespace obscura {

    void Renderer::draw(Terminal& term, const Screen& screen) {
        // Minimal renderer: clear + print glyphs row by row.
        // NOTE: This is not efficient yet; it’s only for bootstrap compilation.
        term.write("\x1b[H");      // cursor home
        if (cfg_.full_redraw && !valid_) {
            term.write("\x1b[2J"); // clear screen
        }

        for (int y = 0; y < screen.rows(); ++y) {
            for (int x = 0; x < screen.cols(); ++x) {
                term.write(screen.at(x, y).glyph);
            }
            term.write("\r\n");
        }
        term.flush();
        valid_ = true;
    }

} // namespace obscura
