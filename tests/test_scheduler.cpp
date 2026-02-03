#include "obscura/ecology/Scheduler.hpp"
#include "obscura/ecology/World.hpp"
#include "test_support.hpp"
#include <chrono>
#include <thread>

namespace {
    struct OrderAgent final : obscura::Agent {
        int id = 0;
        std::vector<int>* order = nullptr;

        obscura::ViewSpec view_spec(obscura::WorldSize) const override {
            return obscura::ViewSpec{0, 0, 1};
        }

        void tick(const obscura::LocalView&, std::vector<obscura::Claim>&) override {
            if (order) order->push_back(id);
        }
    };

    struct ClaimSpamAgent final : obscura::Agent {
        int count = 0;

        obscura::ViewSpec view_spec(obscura::WorldSize) const override {
            return obscura::ViewSpec{0, 0, 1};
        }

        void tick(const obscura::LocalView&, std::vector<obscura::Claim>& out_claims) override {
            for (int i = 0; i < count; ++i) {
                obscura::Claim c;
                c.x = i;
                c.y = 0;
                c.glyph = "X";
                c.priority = 1;
                out_claims.push_back(c);
            }
        }
    };

    struct SlowAgent final : obscura::Agent {
        int sleep_ms = 0;

        obscura::ViewSpec view_spec(obscura::WorldSize) const override {
            return obscura::ViewSpec{0, 0, 1};
        }

        void tick(const obscura::LocalView&, std::vector<obscura::Claim>& out_claims) override {
            obscura::Claim c;
            c.x = 0;
            c.y = 0;
            c.glyph = "Z";
            c.priority = 1;
            out_claims.push_back(c);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }
    };
} // namespace

void run_scheduler_shuffle_order(TestState& state) {
    obscura::World world(std::make_unique<obscura::RandomScheduler>(123));
    world.resize(10, 10);

    obscura::SchedulerConfig cfg;
    cfg.failure_rate = 0.0;
    cfg.max_claims_per_tick = 0;
    cfg.max_tick_duration = std::chrono::microseconds(0);
    world.scheduler().set_config(cfg);

    std::vector<int> order;
    for (int i = 0; i < 4; ++i) {
        auto agent = std::make_unique<OrderAgent>();
        agent->id = i;
        agent->order = &order;
        world.add_agent(std::move(agent));
    }

    std::vector<std::vector<int>> sequences;
    for (int t = 0; t < 6; ++t) {
        order.clear();
        world.tick();
        sequences.push_back(order);
    }

    bool varied = false;
    for (std::size_t i = 1; i < sequences.size(); ++i) {
        if (sequences[i] != sequences[0]) {
            varied = true;
            break;
        }
    }

    CHECK(state, varied);
}

void run_scheduler_claim_budget(TestState& state) {
    obscura::World world(std::make_unique<obscura::RandomScheduler>(7));
    world.resize(10, 2);

    obscura::SchedulerConfig cfg;
    cfg.max_claims_per_tick = 3;
    cfg.failure_rate = 0.0;
    cfg.max_tick_duration = std::chrono::microseconds(0);
    world.scheduler().set_config(cfg);

    auto agent = std::make_unique<ClaimSpamAgent>();
    agent->count = 10;
    world.add_agent(std::move(agent));

    world.tick();

    CHECK(state, world.stats().claims_emitted == 3);
    CHECK(state, world.stats().claim_budget_hits == 1);
    CHECK(state, world.stats().agents.size() == 1);
    CHECK(state, world.stats().agents[0].claim_budget_hits == 1);
    CHECK(state, world.stats().agents[0].claims_emitted == 3);
}

void run_scheduler_time_budget(TestState& state) {
    obscura::World world(std::make_unique<obscura::RandomScheduler>(9));
    world.resize(10, 2);

    obscura::SchedulerConfig cfg;
    cfg.max_claims_per_tick = 0;
    cfg.max_tick_duration = std::chrono::microseconds(500);
    cfg.failure_rate = 0.0;
    world.scheduler().set_config(cfg);

    auto agent = std::make_unique<SlowAgent>();
    agent->sleep_ms = 3;
    world.add_agent(std::move(agent));

    world.tick();

    CHECK(state, world.stats().time_budget_hits == 1);
    CHECK(state, world.stats().agents.size() == 1);
    CHECK(state, world.stats().agents[0].time_budget_hits == 1);
    CHECK(state, world.stats().claims_emitted == 0);
}

void run_scheduler_failure_rate(TestState& state) {
    obscura::World world(std::make_unique<obscura::RandomScheduler>(11));
    world.resize(10, 2);

    obscura::SchedulerConfig cfg;
    cfg.failure_rate = 1.0;
    cfg.max_claims_per_tick = 0;
    cfg.max_tick_duration = std::chrono::microseconds(0);
    world.scheduler().set_config(cfg);

    auto agent = std::make_unique<ClaimSpamAgent>();
    agent->count = 5;
    world.add_agent(std::move(agent));

    world.tick();

    CHECK(state, world.stats().skipped_failures == 1);
    CHECK(state, world.stats().agents.size() == 1);
    CHECK(state, world.stats().agents[0].skipped_failures == 1);
    CHECK(state, world.stats().claims_emitted == 0);
}
