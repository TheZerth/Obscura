#pragma once
#include <string_view>

namespace obscura {

    class Terminal {
    public:
        struct Impl;
        static Terminal open();
        Terminal() = default;
        Terminal(Terminal&&) noexcept;
        Terminal& operator=(Terminal&&) noexcept;
        ~Terminal();

        bool is_tty() const noexcept { return is_tty_; }

        void write(std::string_view bytes);
        void flush();
    private:
        Impl* p_ = nullptr;
        bool is_tty_ = false;
    };

} // namespace obscura
