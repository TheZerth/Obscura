#include "obscura/ecology/Agent.hpp"
#include "obscura/ecology/World.hpp"
#include "test_support.hpp"

#include <memory>

namespace {
    struct EmitAgent final : obscura::Agent {
        void tick(obscura::World& world) override {
            obscura::Claim c1;
            c1.x = 2;
            c1.y = 2;
            c1.glyph = "X";
            c1.priority = 1;
            world.emit(c1);

            obscura::Claim c2;
            c2.x = 2;
            c2.y = 2;
            c2.glyph = "Y";
            c2.priority = 1;
            world.emit(c2);

            obscura::Claim c3;
            c3.x = 4;
            c3.y = 1;
            c3.glyph = "Z";
            c3.priority = 1;
            world.emit(c3);
        }
    };

    struct DoubleContentionAgent final : obscura::Agent {
        void tick(obscura::World& world) override {
            obscura::Claim a;
            a.x = 1;
            a.y = 1;
            a.glyph = "A";
            a.priority = 1;
            world.emit(a);
            obscura::Claim b = a;
            b.glyph = "B";
            world.emit(b);

            obscura::Claim c;
            c.x = 3;
            c.y = 3;
            c.glyph = "C";
            c.priority = 1;
            world.emit(c);
            obscura::Claim d = c;
            d.glyph = "D";
            world.emit(d);
        }
    };
} // namespace

void run_world_contention_single_cell(TestState& state) {
    obscura::World world;
    world.resize(10, 10);
    world.add_agent(std::make_unique<EmitAgent>());
    world.tick();
    CHECK(state, world.stats().contention_cells == 1);
}

void run_world_contention_multiple_cells(TestState& state) {
    obscura::World world;
    world.resize(10, 10);
    world.add_agent(std::make_unique<DoubleContentionAgent>());
    world.tick();
    CHECK(state, world.stats().contention_cells == 2);
}
