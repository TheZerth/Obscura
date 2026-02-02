#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace obscura {

    struct Cell {
        std::string glyph = " ";
        // Style will be extended later; keep minimal for now.
    };

    class Screen {
    public:
        Screen() = default;
        Screen(int cols, int rows) { resize(cols, rows); }

        int cols() const noexcept { return cols_; }
        int rows() const noexcept { return rows_; }

        void resize(int cols, int rows);
        void clear();

        Cell& at(int x, int y);
        const Cell& at(int x, int y) const;

        void put(int x, int y, std::string_view utf8);

    private:
        int cols_ = 0;
        int rows_ = 0;
        std::vector<Cell> cells_; // row-major

        int idx_(int x, int y) const noexcept { return y * cols_ + x; }
    };

} // namespace obscura
