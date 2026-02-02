#include "obscura/term/Terminal.hpp"
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace obscura {

    struct Terminal::Impl {
        int fd_in = STDIN_FILENO;
        int fd_out = STDOUT_FILENO;
        termios orig{};
        bool raw_enabled = false;
        bool cursor_hidden = false;
        bool is_tty = false;

        void restore() {
            if (raw_enabled) {
                tcsetattr(fd_in, TCSAFLUSH, &orig);
                raw_enabled = false;
            }
            if (cursor_hidden) {
                const char* show = "\x1b[?25h";
                ::write(fd_out, show, std::strlen(show));
                cursor_hidden = false;
            }
            ::fsync(fd_out);
        }
    };

    namespace {
        Terminal::Impl* g_active = nullptr;
        bool g_handlers_installed = false;

        void restore_active() {
            if (!g_active) return;
            g_active->restore();
        }

        void on_signal(int sig) {
            restore_active();
            std::signal(sig, SIG_DFL);
            std::raise(sig);
        }
    } // namespace

    Terminal Terminal::open() {
        Terminal t;
        auto* impl = new Terminal::Impl{};
        impl->is_tty = (::isatty(impl->fd_out) == 1);
        t.is_tty_ = impl->is_tty;
        if (!impl->is_tty) {
            delete impl;
            return t;
        }

        if (tcgetattr(impl->fd_in, &impl->orig) == 0) {
            termios raw = impl->orig;
            raw.c_iflag &= static_cast<tcflag_t>(~(BRKINT | ICRNL | INPCK | ISTRIP | IXON));
            raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON | IEXTEN));
            raw.c_lflag |= ISIG;
            raw.c_cc[VMIN] = 1;
            raw.c_cc[VTIME] = 0;
            if (tcsetattr(impl->fd_in, TCSAFLUSH, &raw) == 0) {
                impl->raw_enabled = true;
            }
        }

        const char* hide = "\x1b[?25l";
        ::write(impl->fd_out, hide, std::strlen(hide));
        impl->cursor_hidden = true;
        ::fsync(impl->fd_out);

        t.p_ = impl;

        g_active = impl;
        if (!g_handlers_installed) {
            std::atexit(restore_active);
            std::signal(SIGINT, on_signal);
            std::signal(SIGTERM, on_signal);
            std::signal(SIGHUP, on_signal);
            std::signal(SIGQUIT, on_signal);
            g_handlers_installed = true;
        }
        return t;
    }

    Terminal::Terminal(Terminal&& other) noexcept {
        p_ = other.p_;
        is_tty_ = other.is_tty_;
        other.p_ = nullptr;
        other.is_tty_ = false;
    }

    Terminal& Terminal::operator=(Terminal&& other) noexcept {
        if (this == &other) return *this;
        if (p_) {
            p_->restore();
            if (g_active == p_) g_active = nullptr;
            delete p_;
        }
        p_ = other.p_;
        is_tty_ = other.is_tty_;
        other.p_ = nullptr;
        other.is_tty_ = false;
        return *this;
    }

    Terminal::~Terminal() {
        if (!p_) return;
        p_->restore();
        if (g_active == p_) g_active = nullptr;
        delete p_;
    }

    void Terminal::write(std::string_view bytes) {
        if (!p_) {
            std::cout << bytes;
            return;
        }
        ::write(p_->fd_out, bytes.data(), bytes.size());
    }

    void Terminal::flush() {
        if (!p_) {
            std::cout << std::flush;
            return;
        }
        ::fsync(p_->fd_out);
    }

} // namespace obscura
