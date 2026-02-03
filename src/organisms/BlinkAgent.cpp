#include "obscura/organisms/BlinkAgent.hpp"
#include <algorithm>

namespace obscura {

    namespace {
        std::uint64_t seed_from_device() {
            std::random_device rd;
            std::uint64_t seed = static_cast<std::uint64_t>(rd());
            seed = (seed << 32) ^ static_cast<std::uint64_t>(rd());
            return seed;
        }

    } // namespace

    BlinkAgent::BlinkAgent(BlinkConfig cfg_in) : cfg(std::move(cfg_in)), rng_(seed_from_device()) {
        sanitize_();
    }

    BlinkAgent::BlinkAgent(BlinkConfig cfg_in, std::uint64_t seed)
        : cfg(std::move(cfg_in)), rng_(seed) {
        sanitize_();
    }

    void BlinkAgent::sanitize_() {
        cfg.radius = std::max(0, cfg.radius);
        cfg.on_ticks = std::max(0, cfg.on_ticks);
        cfg.off_ticks = std::max(0, cfg.off_ticks);
        if (cfg.glyph.empty()) cfg.glyph = "*";
        if (cfg.glyphs.empty()) cfg.glyphs = cfg.glyph;
        if (cfg.mutate_every < 0) cfg.mutate_every = 0;
    }

    ViewSpec BlinkAgent::view_spec(WorldSize) const {
        int radius = std::max(0, cfg.radius);
        return ViewSpec{cfg.center_x, cfg.center_y, radius};
    }

    void BlinkAgent::tick(const LocalView& view, std::vector<Claim>& out_claims) {
        tick_counter_++;

        int cycle = cfg.on_ticks + cfg.off_ticks;
        bool on = (cycle <= 0) ? true : ((tick_counter_ - 1) % cycle) < cfg.on_ticks;
        if (!on) return;

        if (cfg.mutate_every > 0 && (tick_counter_ % cfg.mutate_every) == 0) {
            glyph_index_ = (glyph_index_ + 1) % cfg.glyphs.size();
            cfg.glyph = std::string(1, cfg.glyphs[glyph_index_]);
        }

        view.for_each_neighbour(cfg.radius, [&](int x, int y, std::string_view) {
            Claim c;
            c.x = x;
            c.y = y;
            c.glyph = cfg.glyph;
            c.priority = cfg.priority;
            out_claims.push_back(std::move(c));
        });
    }

    BlinkConfig random_blink_config(const BlinkRange& range, std::mt19937_64& rng, WorldSize world) {
        std::uniform_int_distribution<int> radius_dist(range.min_radius, range.max_radius);
        std::uniform_int_distribution<int> on_dist(range.min_on_ticks, range.max_on_ticks);
        std::uniform_int_distribution<int> off_dist(range.min_off_ticks, range.max_off_ticks);
        std::uniform_int_distribution<int> priority_dist(range.min_priority, range.max_priority);

        BlinkConfig cfg;
        cfg.radius = std::max(0, radius_dist(rng));
        cfg.on_ticks = std::max(0, on_dist(rng));
        cfg.off_ticks = std::max(0, off_dist(rng));
        cfg.priority = priority_dist(rng);
        cfg.glyphs = range.glyphs.empty() ? "*" : range.glyphs;
        cfg.glyph = std::string(1, cfg.glyphs[static_cast<std::size_t>(rng() % cfg.glyphs.size())]);

        if (world.cols > 0) {
            std::uniform_int_distribution<int> x_dist(0, world.cols - 1);
            cfg.center_x = x_dist(rng);
        }
        if (world.rows > 0) {
            std::uniform_int_distribution<int> y_dist(0, world.rows - 1);
            cfg.center_y = y_dist(rng);
        }

        return cfg;
    }

    std::unique_ptr<BlinkAgent> make_random_blink_agent(const BlinkRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world) {
        BlinkConfig cfg = random_blink_config(range, rng, world);
        return std::make_unique<BlinkAgent>(cfg, rng());
    }

} // namespace obscura
