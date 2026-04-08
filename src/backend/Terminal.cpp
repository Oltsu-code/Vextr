#include <Vextr/backend/Terminal.hpp>
#include <Vextr/utils/Ansi.hpp>
#include <iostream>
#include <sstream>

#if defined(_WIN32)
    #include <windows.h>
    static DWORD originalConsoleMode;
#else
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
    static struct termios originalTermios;
#endif

namespace vextr::backend {

Terminal::Terminal() {}

Terminal::~Terminal() {
    restore();
}

void Terminal::setup() {
#if defined(_WIN32)
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &originalConsoleMode);
    SetConsoleMode(hIn, ENABLE_VIRTUAL_TERMINAL_INPUT);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outMode;
    GetConsoleMode(hOut, &outMode);
    SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
    tcgetattr(STDIN_FILENO, &originalTermios);
    struct termios raw = originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON); // read char by char
    raw.c_cc[VMIN]  = 0;             // nonblocking
    raw.c_cc[VTIME] = 1;             // 100 ms timeout
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif

    std::cout << utils::ansi::hide_cursor();
    std::cout << utils::ansi::clear_screen();
    std::cout << utils::ansi::clear_scrollback();
    std::cout.flush();
}

void Terminal::restore() {
#if defined(_WIN32)
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, originalConsoleMode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
#endif

    std::cout << utils::ansi::show_cursor();
    std::cout << utils::ansi::clear_screen();
    std::cout << utils::ansi::clear_scrollback();
    std::cout << utils::ansi::reset();
    std::cout << utils::ansi::cursor(0, 0);
    std::cout.flush();
}

void Terminal::clear() {
    std::cout << utils::ansi::reset() << utils::ansi::clear_screen() << utils::ansi::cursor(0, 0);
}

void Terminal::clear_scrollback() {
    std::cout << utils::ansi::clear_scrollback();
}

void Terminal::flush(const Buffer& buf) {
    std::ostringstream out;

    // move to top left once
    out << utils::ansi::cursor(1, 1);

    core::Color lastFg = {255, 255, 255};
    core::Color lastBg = {0, 0, 0};
    bool lastBold      = false;
    bool lastUnderline = false;

    for (int y = 0; y < buf.height(); ++y) {
        for (int x = 0; x < buf.width(); ++x) {
            const Cell& cell = buf.get(x, y);

            // only emit color codes when they change
            if (cell.fg.r != lastFg.r || cell.fg.g != lastFg.g || cell.fg.b != lastFg.b) {
                out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
                lastFg = cell.fg;
            }
            if (cell.bg.r != lastBg.r || cell.bg.g != lastBg.g || cell.bg.b != lastBg.b) {
                out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
                lastBg = cell.bg;
            }
            if (cell.bold != lastBold || cell.underline != lastUnderline) {
                out << utils::ansi::reset();
                if (cell.bold)      out << utils::ansi::bold();
                if (cell.underline) out << utils::ansi::underline();
                // re emit colors after reset
                out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
                out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
                lastBold      = cell.bold;
                lastUnderline = cell.underline;
            }

            out << cell.ch;

        }
        // dont print new line on last row or it will scroll
        if (y < buf.height() - 1) out << '\n';
    }

    std::cout << out.str();
    std::cout.flush();
}

vextr::core::Size Terminal::terminalSize() const {
#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    return {
        csbi.srWindow.Right  - csbi.srWindow.Left + 1,
        csbi.srWindow.Bottom - csbi.srWindow.Top  + 1
    };
#else
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return { ws.ws_col, ws.ws_row };
#endif
}

} // vextr::backend