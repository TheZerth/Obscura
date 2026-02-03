#include "obscura/organisms/NoiseAgent.hpp"
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

        double clamp_double(double v, double lo, double hi) {
            return std::max(lo, std::min(v, hi));
        }
    } // namespace

    NoiseAgent::NoiseAgent(NoiseConfig cfg_in) : cfg(std::move(cfg_in)), rng_(seed_from_device()) {
        sanitize_();
    }

    NoiseAgent::NoiseAgent(NoiseConfig cfg_in, std::uint64_t seed)
        : cfg(std::move(cfg_in)), rng_(seed) {
        sanitize_();
    }

    void NoiseAgent::sanitize_() {
        cfg.width = std::max(1, cfg.width);
        cfg.height = std::max(1, cfg.height);
        cfg.density = clamp_double(cfg.density, 0.0, 1.0);
        cfg.update_interval = std::max(1, cfg.update_interval);
        if (cfg.glyphs.empty()) cfg.glyphs = " ";
        if (cfg.mutate_every < 0) cfg.mutate_every = 0;
    }

    ViewSpec NoiseAgent::view_spec(WorldSize) const {
        int radius = std::max(1, std::max(cfg.width, cfg.height) / 2);
        return ViewSpec{cfg.center_x, cfg.center_y, radius};
    }

    void NoiseAgent::tick(const LocalView& view, std::vector<Claim>& out_claims) {
        tick_counter_++;
        if (cfg.update_interval > 1 && (tick_counter_ % cfg.update_interval) != 0) return;

        if (cfg.mutate_every > 0 && (tick_counter_ % cfg.mutate_every) == 0) {
            glyph_offset_ = (glyph_offset_ + 1) % cfg.glyphs.size();
        }

        std::uniform_real_distribution<double> prob(0.0, 1.0);
        std::uniform_int_distribution<std::size_t> pick(0, cfg.glyphs.size() - 1);

        int half_w = cfg.width / 2;
        int half_h = cfg.height / 2;
        int start_x = cfg.center_x - half_w;
        int start_y = cfg.center_y - half_h;

        for (int y = 0; y < cfg.height; ++y) {
            for (int x = 0; x < cfg.width; ++x) {
                if (prob(rng_) > cfg.density) continue;
                int gx = start_x + x;
                int gy = start_y + y;
                if (cfg.avoid_non_empty && view.glyph_at(gx, gy) != " ") continue;

                std::size_t idx = (pick(rng_) + glyph_offset_) % cfg.glyphs.size();
                Claim c;
                c.x = gx;
                c.y = gy;
                c.glyph = std::string(1, cfg.glyphs[idx]);
                c.priority = cfg.priority;
                out_claims.push_back(std::move(c));
            }
        }
    }

    NoiseConfig random_noise_config(const NoiseRange& range, std::mt19937_64& rng, WorldSize world) {
        std::uniform_int_distribution<int> width_dist(range.min_width, range.max_width);
        std::uniform_int_distribution<int> height_dist(range.min_height, range.max_height);
        std::uniform_real_distribution<double> density_dist(range.min_density, range.max_density);
        std::uniform_int_distribution<int> priority_dist(range.min_priority, range.max_priority);
        std::uniform_int_distribution<int> interval_dist(range.min_update_interval, range.max_update_interval);

        NoiseConfig cfg;
        cfg.width = std::max(1, width_dist(rng));
        cfg.height = std::max(1, height_dist(rng));
        cfg.density = clamp_double(density_dist(rng), 0.0, 1.0);
        cfg.priority = priority_dist(rng);
        cfg.update_interval = std::max(1, interval_dist(rng));
        cfg.glyphs = range.glyphs.empty() ? "*" : range.glyphs;
        cfg.mutate_every = 0;

        if (world.cols > 0) {
            std::uniform_int_distribution<int> x_dist(0, world.cols - 1);
            cfg.center_x = x_dist(rng);
        }
        if (world.rows > 0) {
            std::uniform_int_distribution<int> y_dist(0, world.rows - 1);
            cfg.center_y = y_dist(rng);
        }

        cfg.width = clamp_int(cfg.width, 1, std::max(1, world.cols));
        cfg.height = clamp_int(cfg.height, 1, std::max(1, world.rows));
        return cfg;
    }

    std::unique_ptr<NoiseAgent> make_random_noise_agent(const NoiseRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world) {
        NoiseConfig cfg = random_noise_config(range, rng, world);
        return std::make_unique<NoiseAgent>(cfg, rng());
    }

} // namespace obscura
