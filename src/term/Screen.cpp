#include "obscura/term/Screen.hpp"
#include <stdexcept>

namespace obscura {

    void Screen::resize(int cols, int rows) {
        cols_ = (cols < 0) ? 0 : cols;
        rows_ = (rows < 0) ? 0 : rows;
        cells_.assign(static_cast<size_t>(cols_ * rows_), Cell{});
    }

    void Screen::clear() {
        for (auto& c : cells_) c = Cell{};
    }

    Cell& Screen::at(int x, int y) {
        if (x < 0 || y < 0 || x >= cols_ || y >= rows_) {
            throw std::out_of_range("Screen::at out of range");
        }
        return cells_[static_cast<size_t>(idx_(x, y))];
    }

    const Cell& Screen::at(int x, int y) const {
        if (x < 0 || y < 0 || x >= cols_ || y >= rows_) {
            throw std::out_of_range("Screen::at out of range");
        }
        return cells_[static_cast<size_t>(idx_(x, y))];
    }

    void Screen::put(int x, int y, std::string_view utf8) {
        if (x < 0 || y < 0 || x >= cols_ || y >= rows_) return;
        cells_[static_cast<size_t>(idx_(x, y))].glyph = std::string(utf8);
    }

} // namespace obscura
