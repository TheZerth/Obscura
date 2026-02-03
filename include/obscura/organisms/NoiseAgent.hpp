#pragma once
#include "obscura/ecology/Agent.hpp"
#include <memory>
#include <random>
#include <string>

namespace obscura {

    struct NoiseConfig {
        int center_x = 0;
        int center_y = 0;
        int width = 8;
        int height = 4;
        double density = 0.15; // 0..1
        std::string glyphs = ".*";
        int priority = 0;
        int update_interval = 1;
        bool avoid_non_empty = false;
        int mutate_every = 0;
    };

    struct NoiseRange {
        int min_width = 3;
        int max_width = 12;
        int min_height = 2;
        int max_height = 8;
        double min_density = 0.05;
        double max_density = 0.25;
        std::string glyphs = ".*";
        int min_priority = 0;
        int max_priority = 5;
        int min_update_interval = 1;
        int max_update_interval = 4;
    };

    class NoiseAgent final : public Agent {
    public:
        explicit NoiseAgent(NoiseConfig cfg);
        NoiseAgent(NoiseConfig cfg, std::uint64_t seed);

        const char* name() const noexcept override { return "NoiseAgent"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;

        NoiseConfig cfg;

    private:
        std::mt19937_64 rng_;
        int tick_counter_ = 0;
        std::size_t glyph_offset_ = 0;

        void sanitize_();
    };

    NoiseConfig random_noise_config(const NoiseRange& range, std::mt19937_64& rng, WorldSize world);
    std::unique_ptr<NoiseAgent> make_random_noise_agent(const NoiseRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world);

} // namespace obscura
