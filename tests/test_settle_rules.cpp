#include "obscura/ecology/CellSettle.hpp"
#include "obscura/ecology/Claim.hpp"
#include "test_support.hpp"

#include <vector>

void run_cell_settle_tie_break(TestState& state) {
    obscura::CellSettle settle;
    std::vector<obscura::Claim> claims;
    claims.push_back({0, 0, "b", 5});
    claims.push_back({0, 0, "a", 5});
    claims.push_back({0, 0, "c", 5});

    int idx = settle.choose(claims);
    CHECK(state, idx >= 0);
    CHECK(state, claims[static_cast<size_t>(idx)].glyph == "a");
}

void run_cell_settle_priority(TestState& state) {
    obscura::CellSettle settle;
    std::vector<obscura::Claim> claims;
    claims.push_back({0, 0, "a", 1});
    claims.push_back({0, 0, "b", 3});
    claims.push_back({0, 0, "c", 2});

    int idx = settle.choose(claims);
    CHECK(state, idx >= 0);
    CHECK(state, claims[static_cast<size_t>(idx)].glyph == "b");
}

void run_cell_settle_determinism(TestState& state) {
    obscura::CellSettle settle;
    std::vector<obscura::Claim> claims;
    claims.push_back({0, 0, "b", 2});
    claims.push_back({0, 0, "a", 2});
    claims.push_back({0, 0, "c", 2});

    int first = settle.choose(claims);
    CHECK(state, first >= 0);
    for (int i = 0; i < 10; ++i) {
        int again = settle.choose(claims);
        CHECK(state, again == first);
    }

    std::vector<obscura::Claim> shuffled;
    shuffled.push_back({0, 0, "c", 2});
    shuffled.push_back({0, 0, "b", 2});
    shuffled.push_back({0, 0, "a", 2});
    int idx = settle.choose(shuffled);
    CHECK(state, idx >= 0);
    CHECK(state, shuffled[static_cast<size_t>(idx)].glyph == "a");
}
