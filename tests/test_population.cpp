#include "obscura/ecology/World.hpp"
#include "obscura/organisms/NoiseAgent.hpp"
#include "obscura/organisms/BlinkAgent.hpp"
#include "obscura/organisms/TrailAgent.hpp"
#include "test_support.hpp"
#include <random>

namespace {
    struct DummyAgent final : obscura::Agent {
        const char* name() const noexcept override { return "Dummy"; }
        obscura::ViewSpec view_spec(obscura::WorldSize) const override {
            return obscura::ViewSpec{0, 0, 1};
        }
        void tick(const obscura::LocalView&, std::vector<obscura::Claim>&) override {}
    };
} // namespace

void run_population_random_bounds(TestState& state) {
    std::mt19937_64 rng(123);
    obscura::WorldSize world{80, 24};

    obscura::NoiseRange nrange;
    nrange.min_width = 2;
    nrange.max_width = 6;
    nrange.min_height = 1;
    nrange.max_height = 5;
    nrange.min_density = 0.1;
    nrange.max_density = 0.2;
    nrange.min_priority = 1;
    nrange.max_priority = 3;
    nrange.min_update_interval = 1;
    nrange.max_update_interval = 3;

    auto ncfg = obscura::random_noise_config(nrange, rng, world);
    CHECK(state, ncfg.width >= nrange.min_width && ncfg.width <= nrange.max_width);
    CHECK(state, ncfg.height >= nrange.min_height && ncfg.height <= nrange.max_height);
    CHECK(state, ncfg.density >= nrange.min_density && ncfg.density <= nrange.max_density);
    CHECK(state, ncfg.priority >= nrange.min_priority && ncfg.priority <= nrange.max_priority);
    CHECK(state, ncfg.update_interval >= nrange.min_update_interval && ncfg.update_interval <= nrange.max_update_interval);
    CHECK(state, ncfg.center_x >= 0 && ncfg.center_x < world.cols);
    CHECK(state, ncfg.center_y >= 0 && ncfg.center_y < world.rows);

    obscura::BlinkRange brange;
    brange.min_radius = 0;
    brange.max_radius = 2;
    brange.min_on_ticks = 1;
    brange.max_on_ticks = 4;
    brange.min_off_ticks = 1;
    brange.max_off_ticks = 4;
    brange.min_priority = 0;
    brange.max_priority = 2;

    auto bcfg = obscura::random_blink_config(brange, rng, world);
    CHECK(state, bcfg.radius >= brange.min_radius && bcfg.radius <= brange.max_radius);
    CHECK(state, bcfg.on_ticks >= brange.min_on_ticks && bcfg.on_ticks <= brange.max_on_ticks);
    CHECK(state, bcfg.off_ticks >= brange.min_off_ticks && bcfg.off_ticks <= brange.max_off_ticks);
    CHECK(state, bcfg.priority >= brange.min_priority && bcfg.priority <= brange.max_priority);
    CHECK(state, bcfg.center_x >= 0 && bcfg.center_x < world.cols);
    CHECK(state, bcfg.center_y >= 0 && bcfg.center_y < world.rows);

    obscura::TrailRange trange;
    trange.min_length = 3;
    trange.max_length = 9;
    trange.min_priority = 1;
    trange.max_priority = 4;
    trange.min_update_interval = 1;
    trange.max_update_interval = 2;

    auto tcfg = obscura::random_trail_config(trange, rng, world);
    CHECK(state, tcfg.length >= trange.min_length && tcfg.length <= trange.max_length);
    CHECK(state, tcfg.priority >= trange.min_priority && tcfg.priority <= trange.max_priority);
    CHECK(state, tcfg.update_interval >= trange.min_update_interval && tcfg.update_interval <= trange.max_update_interval);
    CHECK(state, tcfg.start_x >= 0 && tcfg.start_x < world.cols);
    CHECK(state, tcfg.start_y >= 0 && tcfg.start_y < world.rows);
}

void run_population_remove_agents(TestState& state) {
    obscura::World world;
    world.resize(10, 10);

    auto id1 = world.add_agent(std::make_unique<DummyAgent>());
    auto id2 = world.add_agent(std::make_unique<DummyAgent>());
    CHECK(state, id1 != id2);
    CHECK(state, world.stats().agents.size() == 2);
    CHECK(state, world.stats().agents[0].active == true);
    CHECK(state, world.stats().agents[1].active == true);

    CHECK(state, world.remove_agent(id1) == true);
    CHECK(state, world.stats().agents[0].active == false);
    CHECK(state, world.agent_by_id(id1) == nullptr);
    CHECK(state, world.agent_by_id(id2) != nullptr);
    CHECK(state, world.remove_agent(999999) == false);
}
