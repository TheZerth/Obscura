#pragma once
#include <cstdint>
#include <optional>
#include <string>

namespace obscura {

    enum class Key : uint16_t {
        unknown = 0,
        enter, esc, backspace, tab,
        up, down, left, right,
      };

    enum Mod : uint8_t {
        mod_none  = 0,
        mod_shift = 1 << 0,
        mod_alt   = 1 << 1,
        mod_ctrl  = 1 << 2,
      };

    struct KeyEvent {
        Key key = Key::unknown;
        uint8_t mod = mod_none;
        std::string text; // UTF-8 if applicable
    };

    struct ResizeEvent {
        int cols = 0;
        int rows = 0;
    };

    struct Event {
        enum class Type : uint8_t { none, key, resize, tick, eof } type = Type::none;
        KeyEvent key;
        ResizeEvent resize;
    };

    struct InputConfig {
        bool utf8 = true;
    };

    class Input {
    public:
        explicit Input(int fd_in, InputConfig cfg = {});
        std::optional<Event> read_event(); // minimal: currently returns nullopt

    private:
        int fd_ = -1;
        InputConfig cfg_;
    };

} // namespace obscura
