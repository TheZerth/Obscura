#pragma once
#include "obscura/term/Screen.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace obscura {

    class LocalView {
    public:
        LocalView(const Screen& screen, int center_x, int center_y, int radius)
            : screen_(&screen), center_x_(center_x), center_y_(center_y), radius_(radius < 0 ? 0 : radius) {}

        int center_x() const noexcept { return center_x_; }
        int center_y() const noexcept { return center_y_; }
        int radius() const noexcept { return radius_; }

        // Note: returned string_view is valid only until the end of the current tick.
        std::string_view glyph_at(int x, int y) const {
            if (!in_view_(x, y) || !in_bounds_(x, y)) return default_glyph_;
            return screen_->at(x, y).glyph;
        }

        // Calls fn(x, y, glyph) for each cell in the neighbourhood (row-major).
        // Note: glyph string_views are valid only until the end of the current tick.
        template <class F>
        void for_each_neighbour(int r, F&& fn) const {
            int rr = std::max(0, r);
            if (rr > radius_) rr = radius_;
            if (rr == 0) {
                std::forward<F>(fn)(center_x_, center_y_, glyph_at(center_x_, center_y_));
                return;
            }
            for (int y = center_y_ - rr; y <= center_y_ + rr; ++y) {
                for (int x = center_x_ - rr; x <= center_x_ + rr; ++x) {
                    std::forward<F>(fn)(x, y, glyph_at(x, y));
                }
            }
        }

        // Note: returned string_views are valid only until the end of the current tick.
        std::vector<std::string_view> neighbours(int r) const {
            std::vector<std::string_view> out;
            int rr = std::max(0, r);
            if (rr > radius_) rr = radius_;
            out.reserve(static_cast<size_t>((2 * rr + 1) * (2 * rr + 1)));
            for_each_neighbour(rr, [&](int, int, std::string_view glyph) {
                out.push_back(glyph);
            });
            return out;
        }

    private:
        const Screen* screen_ = nullptr;
        int center_x_ = 0;
        int center_y_ = 0;
        int radius_ = 0;
        std::string default_glyph_ = " ";

        bool in_bounds_(int x, int y) const noexcept {
            return x >= 0 && y >= 0 && x < screen_->cols() && y < screen_->rows();
        }

        bool in_view_(int x, int y) const noexcept {
            return std::abs(x - center_x_) <= radius_ && std::abs(y - center_y_) <= radius_;
        }
    };

} // namespace obscura
