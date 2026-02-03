#pragma once
#include "obscura/ecology/Agent.hpp"
#include <deque>
#include <memory>
#include <random>
#include <string>
#include <utility>

namespace obscura {

    struct TrailConfig {
        int start_x = 0;
        int start_y = 0;
        int dx = 1;
        int dy = 0;
        int length = 8;
        std::string glyph = "o";
        std::string glyphs = "o";
        int priority = 0;
        int update_interval = 1;
        int sense_radius = 1;
        bool turn_on_block = true;
        int mutate_every = 0;
    };

    struct TrailRange {
        int min_length = 4;
        int max_length = 16;
        int min_priority = 0;
        int max_priority = 6;
        int min_update_interval = 1;
        int max_update_interval = 3;
        std::string glyphs = "o@";
    };

    class TrailAgent final : public Agent {
    public:
        explicit TrailAgent(TrailConfig cfg);
        TrailAgent(TrailConfig cfg, std::uint64_t seed);

        const char* name() const noexcept override { return "TrailAgent"; }
        ViewSpec view_spec(WorldSize world) const override;
        void tick(const LocalView& view, std::vector<Claim>& out_claims) override;

        TrailConfig cfg;

    private:
        std::mt19937_64 rng_;
        int tick_counter_ = 0;
        std::deque<std::pair<int, int>> trail_;
        mutable WorldSize last_world_{0, 0};
        std::size_t glyph_index_ = 0;

        void sanitize_();
        void wrap_(int& x, int& y) const;
        void turn_right_();
        void random_turn_();
    };

    TrailConfig random_trail_config(const TrailRange& range, std::mt19937_64& rng, WorldSize world);
    std::unique_ptr<TrailAgent> make_random_trail_agent(const TrailRange& range,
                                                        std::mt19937_64& rng,
                                                        WorldSize world);

} // namespace obscura
