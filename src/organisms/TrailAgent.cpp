#include "obscura/organisms/TrailAgent.hpp"
#include <algorithm>

namespace obscura {

    namespace {
        std::uint64_t seed_from_device() {
            std::random_device rd;
            std::uint64_t seed = static_cast<std::uint64_t>(rd());
            seed = (seed << 32) ^ static_cast<std::uint64_t>(rd());
            return seed;
        }

        int clamp_int(int v, int lo, int hi) {
            return std::max(lo, std::min(v, hi));
        }
    } // namespace

    TrailAgent::TrailAgent(TrailConfig cfg_in) : cfg(std::move(cfg_in)), rng_(seed_from_device()) {
        sanitize_();
    }

    TrailAgent::TrailAgent(TrailConfig cfg_in, std::uint64_t seed)
        : cfg(std::move(cfg_in)), rng_(seed) {
        sanitize_();
    }

    void TrailAgent::sanitize_() {
        if (cfg.dx == 0 && cfg.dy == 0) cfg.dx = 1;
        cfg.length = std::max(1, cfg.length);
        cfg.update_interval = std::max(1, cfg.update_interval);
        cfg.sense_radius = std::max(1, cfg.sense_radius);
        if (cfg.glyph.empty()) cfg.glyph = "o";
        if (cfg.glyphs.empty()) cfg.glyphs = cfg.glyph;
        if (cfg.mutate_every < 0) cfg.mutate_every = 0;
    }

    ViewSpec TrailAgent::view_spec(WorldSize world) const {
        last_world_ = world;
        int cx = trail_.empty() ? cfg.start_x : trail_.back().first;
        int cy = trail_.empty() ? cfg.start_y : trail_.back().second;
        return ViewSpec{cx, cy, cfg.sense_radius};
    }

    void TrailAgent::wrap_(int& x, int& y) const {
        if (last_world_.cols > 0) {
            x %= last_world_.cols;
            if (x < 0) x += last_world_.cols;
        }
        if (last_world_.rows > 0) {
            y %= last_world_.rows;
            if (y < 0) y += last_world_.rows;
        }
    }

    void TrailAgent::turn_right_() {
        int ndx = -cfg.dy;
        int ndy = cfg.dx;
        cfg.dx = (ndx == 0 && ndy == 0) ? 1 : ndx;
        cfg.dy = ndy;
    }

    void TrailAgent::random_turn_() {
        static const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        std::uniform_int_distribution<int> pick(0, 3);
        int idx = pick(rng_);
        cfg.dx = dirs[idx][0];
        cfg.dy = dirs[idx][1];
    }

    void TrailAgent::tick(const LocalView& view, std::vector<Claim>& out_claims) {
        tick_counter_++;
        if (cfg.update_interval > 1 && (tick_counter_ % cfg.update_interval) != 0) return;

        if (cfg.mutate_every > 0 && (tick_counter_ % cfg.mutate_every) == 0) {
            glyph_index_ = (glyph_index_ + 1) % cfg.glyphs.size();
            cfg.glyph = std::string(1, cfg.glyphs[glyph_index_]);
        }

        if (trail_.empty()) {
            trail_.push_back({cfg.start_x, cfg.start_y});
            wrap_(trail_.back().first, trail_.back().second);
        }

        auto [hx, hy] = trail_.back();
        int nx = hx + cfg.dx;
        int ny = hy + cfg.dy;
        wrap_(nx, ny);

        if (cfg.turn_on_block && view.glyph_at(nx, ny) != " ") {
            turn_right_();
            nx = hx + cfg.dx;
            ny = hy + cfg.dy;
            wrap_(nx, ny);
            if (view.glyph_at(nx, ny) != " ") {
                random_turn_();
                nx = hx + cfg.dx;
                ny = hy + cfg.dy;
                wrap_(nx, ny);
            }
        }

        trail_.push_back({nx, ny});
        while (static_cast<int>(trail_.size()) > cfg.length) {
            trail_.pop_front();
        }

        for (const auto& [x, y] : trail_) {
            Claim c;
            c.x = x;
            c.y = y;
            c.glyph = cfg.glyph;
            c.priority = cfg.priority;
            out_claims.push_back(std::move(c));
        }
    }

    TrailConfig random_trail_config(const TrailRange& range, std::mt19937_64& rng, WorldSize world) {
        std::uniform_int_distribution<int> len_dist(range.min_length, range.max_length);
        std::uniform_int_distribution<int> priority_dist(range.min_priority, range.max_priority);
        std::uniform_int_distribution<int> interval_dist(range.min_update_interval, range.max_update_interval);
        std::uniform_int_distribution<int> dir_dist(0, 3);

        TrailConfig cfg;
        cfg.length = std::max(1, len_dist(rng));
        cfg.priority = priority_dist(rng);
        cfg.update_interval = std::max(1, interval_dist(rng));
        cfg.glyphs = range.glyphs.empty() ? "o" : range.glyphs;
        cfg.glyph = std::string(1, cfg.glyphs[static_cast<std::size_t>(rng() % cfg.glyphs.size())]);

        int dir = dir_dist(rng);
        cfg.dx = (dir == 0) - (dir == 1);
        cfg.dy = (dir == 2) - (dir == 3);
        if (cfg.dx == 0 && cfg.dy == 0) cfg.dx = 1;

        if (world.cols > 0) {
            std::uniform_int_distribution<int> x_dist(0, world.cols - 1);
            cfg.start_x = x_dist(rng);
        }
        if (world.rows > 0) {
            std::uniform_int_distribution<int> y_dist(0, world.rows - 1);
            cfg.start_y = y_dist(rng);
        }

        cfg.length = clamp_int(cfg.length, 1, std::max(1, world.cols * world.rows));
        return cfg;
    }

    std::unique_ptr<TrailAgent> make_random_trail_agent(const TrailRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world) {
        TrailConfig cfg = random_trail_config(range, rng, world);
        return std::make_unique<TrailAgent>(cfg, rng());
    }

} // namespace obscura
