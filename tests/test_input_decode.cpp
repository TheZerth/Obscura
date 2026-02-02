#include "obscura/term/Input.hpp"
#include "test_support.hpp"

#include <cstring>
#include <string>

static obscura::DecodeResult decode_or_fail(TestState& state, const unsigned char* data, std::size_t len) {
    auto decoded = obscura::decode_key_from_buffer(data, len);
    CHECK(state, decoded.has_value());
    if (!decoded) return {};
    return *decoded;
}

void run_input_decode_arrows(TestState& state) {
    const unsigned char up[] = {0x1b, '[', 'A'};
    const unsigned char down[] = {0x1b, '[', 'B'};
    const unsigned char right[] = {0x1b, '[', 'C'};
    const unsigned char left[] = {0x1b, '[', 'D'};

    CHECK(state, decode_or_fail(state, up, sizeof(up)).key.key == obscura::Key::up);
    CHECK(state, decode_or_fail(state, down, sizeof(down)).key.key == obscura::Key::down);
    CHECK(state, decode_or_fail(state, right, sizeof(right)).key.key == obscura::Key::right);
    CHECK(state, decode_or_fail(state, left, sizeof(left)).key.key == obscura::Key::left);
}

void run_input_decode_enter_esc_tab_backspace(TestState& state) {
    const unsigned char cr[] = {'\r'};
    const unsigned char lf[] = {'\n'};
    const unsigned char esc[] = {0x1b};
    const unsigned char tab[] = {'\t'};
    const unsigned char bs_del[] = {0x7f};
    const unsigned char bs_bs[] = {0x08};

    CHECK(state, decode_or_fail(state, cr, sizeof(cr)).key.key == obscura::Key::enter);
    CHECK(state, decode_or_fail(state, lf, sizeof(lf)).key.key == obscura::Key::enter);
    CHECK(state, decode_or_fail(state, esc, sizeof(esc)).key.key == obscura::Key::esc);
    CHECK(state, decode_or_fail(state, tab, sizeof(tab)).key.key == obscura::Key::tab);
    CHECK(state, decode_or_fail(state, bs_del, sizeof(bs_del)).key.key == obscura::Key::backspace);
    CHECK(state, decode_or_fail(state, bs_bs, sizeof(bs_bs)).key.key == obscura::Key::backspace);
}

void run_input_decode_printable(TestState& state) {
    const unsigned char c1[] = {'A'};
    const unsigned char c2[] = {' '};
    auto r1 = decode_or_fail(state, c1, sizeof(c1));
    auto r2 = decode_or_fail(state, c2, sizeof(c2));
    CHECK(state, r1.key.key == obscura::Key::unknown);
    CHECK(state, r1.key.text == "A");
    CHECK(state, r2.key.key == obscura::Key::unknown);
    CHECK(state, r2.key.text == " ");
}
