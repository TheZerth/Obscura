#pragma once
#include <string_view>

namespace obscura {

    class Terminal {
    public:
        static Terminal open();
        Terminal() = default;
        Terminal(Terminal&&) noexcept;
        Terminal& operator=(Terminal&&) noexcept;
        ~Terminal();

        bool is_tty() const noexcept { return true; }

        void write(std::string_view bytes);
        void flush();
    private:
        struct Impl;
        Impl* p_ = nullptr;
    };

} // namespace obscura