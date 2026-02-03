#pragma once
#include "obscura/ecology/Agent.hpp"
#include <memory>
#include <random>
#include <string>

namespace obscura {

    struct BlinkConfig {
        int center_x = 0;
        int center_y = 0;
        int radius = 0;
        std::string glyph = "*";
        int on_ticks = 4;
        int off_ticks = 4;
        int priority = 0;
        int mutate_every = 0;
        std::string glyphs = "*";
    };

    struct BlinkRange {
        int min_radius = 0;
        int max_radius = 3;
        int min_on_ticks = 2;
        int max_on_ticks = 8;
        int min_off_ticks = 2;
        int max_off_ticks = 8;
        int min_priority = 0;
        int max_priority = 6;
        std::string glyphs = "*+";
    };

    class BlinkAgent final : public Agent {
    public:
        explicit BlinkAgent(BlinkConfig cfg);
        BlinkAgent(BlinkConfig cfg, std::uint64_t seed);

        const char* name() const noexcept override { return "BlinkAgent"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;

        BlinkConfig cfg;

    private:
        std::mt19937_64 rng_;
        int tick_counter_ = 0;
        std::size_t glyph_index_ = 0;

        void sanitize_();
    };

    BlinkConfig random_blink_config(const BlinkRange& range, std::mt19937_64& rng, WorldSize world);
    std::unique_ptr<BlinkAgent> make_random_blink_agent(const BlinkRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world);

} // namespace obscura
