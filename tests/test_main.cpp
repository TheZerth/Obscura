#include "test_support.hpp"

void run_cell_settle_tie_break(TestState& state);
void run_cell_settle_priority(TestState& state);
void run_cell_settle_determinism(TestState& state);

void run_input_decode_arrows(TestState& state);
void run_input_decode_enter_esc_tab_backspace(TestState& state);
void run_input_decode_printable(TestState& state);

void run_world_contention_single_cell(TestState& state);
void run_world_contention_multiple_cells(TestState& state);

int main() {
    TestState state;

    run_cell_settle_tie_break(state);
    run_cell_settle_priority(state);
    run_cell_settle_determinism(state);

    run_input_decode_arrows(state);
    run_input_decode_enter_esc_tab_backspace(state);
    run_input_decode_printable(state);

    run_world_contention_single_cell(state);
    run_world_contention_multiple_cells(state);

    return state.failures == 0 ? 0 : 1;
}
