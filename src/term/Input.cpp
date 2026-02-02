#include "obscura/term/Input.hpp"
#include <cerrno>
#include <csignal>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>

namespace obscura {

    namespace {
        volatile sig_atomic_t g_winched = 0;
        bool g_installed = false;

        void on_winch(int) { g_winched = 1; }

        bool query_size(int fd, int& cols, int& rows) {
            winsize ws{};
            if (::ioctl(fd, TIOCGWINSZ, &ws) != 0) return false;
            cols = ws.ws_col;
            rows = ws.ws_row;
            return true;
        }

        bool read_byte_with_timeout(int fd, int timeout_ms, unsigned char& out) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(fd, &set);
            timeval tv{};
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
            int rv = ::select(fd + 1, &set, nullptr, nullptr, &tv);
            if (rv <= 0) return false;
            ssize_t n = ::read(fd, &out, 1);
            return n == 1;
        }
    } // namespace

    Input::Input(int fd_in, InputConfig cfg) : fd_(fd_in), cfg_(cfg) {
        if (!g_installed) {
            std::signal(SIGWINCH, on_winch);
            g_installed = true;
        }
        query_size(fd_, last_cols_, last_rows_);
    }

    std::optional<Event> Input::read_event() {
        for (;;) {
            if (g_winched) {
                g_winched = 0;
                int cols = last_cols_;
                int rows = last_rows_;
                if (query_size(fd_, cols, rows)) {
                    last_cols_ = cols;
                    last_rows_ = rows;
                }
                Event ev;
                ev.type = Event::Type::resize;
                ev.resize = {last_cols_, last_rows_};
                return ev;
            }

            unsigned char ch = 0;
            ssize_t n = ::read(fd_, &ch, 1);
            if (n == 0) {
                Event ev;
                ev.type = Event::Type::eof;
                return ev;
            }
            if (n < 0) {
                if (errno == EINTR) continue;
                return std::nullopt;
            }

            Event ev;
            ev.type = Event::Type::key;

            if (ch == 0x1b) {
                unsigned char b1 = 0;
                if (read_byte_with_timeout(fd_, 0, b1) && b1 == '[') {
                    unsigned char b2 = 0;
                    if (read_byte_with_timeout(fd_, 10, b2)) {
                        switch (b2) {
                            case 'A': ev.key.key = Key::up; return ev;
                            case 'B': ev.key.key = Key::down; return ev;
                            case 'C': ev.key.key = Key::right; return ev;
                            case 'D': ev.key.key = Key::left; return ev;
                            default: break;
                        }
                    }
                }
                ev.key.key = Key::esc;
                return ev;
            }

            if (ch == '\r' || ch == '\n') {
                ev.key.key = Key::enter;
                return ev;
            }
            if (ch == 0x7f || ch == 0x08) {
                ev.key.key = Key::backspace;
                return ev;
            }
            if (ch == '\t') {
                ev.key.key = Key::tab;
                return ev;
            }

            if (ch >= 32 && ch <= 126) {
                ev.key.key = Key::unknown;
                ev.key.text = std::string(1, static_cast<char>(ch));
                return ev;
            }
        }
    }

} // namespace obscura
