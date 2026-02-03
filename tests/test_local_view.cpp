#include "obscura/ecology/LocalView.hpp"
#include "obscura/term/Screen.hpp"
#include "test_support.hpp"

void run_local_view_glyph_at(TestState& state) {
    obscura::Screen screen(5, 5);
    screen.put(2, 2, "X");
    screen.put(1, 1, "A");

    obscura::LocalView view(screen, 2, 2, 1);
    CHECK(state, view.glyph_at(2, 2) == "X");
    CHECK(state, view.glyph_at(1, 1) == "A");
    CHECK(state, view.glyph_at(0, 0) == " ");
    CHECK(state, view.glyph_at(-1, 2) == " ");
}

void run_local_view_neighbours(TestState& state) {
    obscura::Screen screen(5, 5);
    screen.put(2, 2, "X");
    screen.put(1, 2, "L");
    screen.put(3, 2, "R");

    obscura::LocalView view(screen, 2, 2, 1);
    auto neigh = view.neighbours(1);
    CHECK(state, neigh.size() == 9);
    CHECK(state, neigh[4] == "X");
    CHECK(state, neigh[3] == "L");
    CHECK(state, neigh[5] == "R");
}

void run_local_view_for_each_neighbour(TestState& state) {
    obscura::Screen screen(5, 5);
    screen.put(2, 2, "X");
    screen.put(1, 2, "L");
    screen.put(3, 2, "R");

    obscura::LocalView view(screen, 2, 2, 1);
    auto neigh = view.neighbours(1);
    std::vector<std::string_view> via_callback;
    via_callback.reserve(neigh.size());
    view.for_each_neighbour(1, [&](int, int, std::string_view glyph) {
        via_callback.push_back(glyph);
    });
    CHECK(state, via_callback == neigh);
}
