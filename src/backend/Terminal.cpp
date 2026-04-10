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

Terminal::~Terminal() { restore(); }

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
  raw.c_cc[VMIN] = 0;              // nonblocking
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
  std::cout << utils::ansi::reset() << utils::ansi::clear_screen()
            << utils::ansi::cursor(0, 0);
}

void Terminal::clear_scrollback() {
  std::cout << utils::ansi::clear_scrollback();
}

#include <cstdio>

void Terminal::write(std::string_view bytes) {
  std::fwrite(bytes.data(), 1, bytes.size(), stdout);
  std::fflush(stdout);
}

vextr::core::Size Terminal::terminalSize() const {
#if defined(_WIN32)
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hOut, &csbi);
  return {csbi.srWindow.Right - csbi.srWindow.Left + 1,
          csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
#else
  struct winsize ws;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  return {ws.ws_col, ws.ws_row};
#endif
}

} // namespace vextr::backend