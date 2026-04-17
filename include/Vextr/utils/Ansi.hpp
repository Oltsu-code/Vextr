#pragma once
#include <string>

/// @namespace vextr::utils::ansi
/// @brief Utility functions for generating ANSI escape codes for terminal control and styling.
namespace vextr::utils::ansi {

// cursor controls
inline std::string cursor(int row, int col) {
  return "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}
inline std::string move_up(int n) { return "\033[" + std::to_string(n) + "A"; }
inline std::string move_down(int n) {
  return "\033[" + std::to_string(n) + "B";
}
inline std::string move_right(int n) {
  return "\033[" + std::to_string(n) + "C";
}
inline std::string move_left(int n) {
  return "\033[" + std::to_string(n) + "D";
}
inline std::string hide_cursor() { return "\033[?25l"; }
inline std::string show_cursor() { return "\033[?25h"; }

// terminal controls
inline std::string clear_screen() { return "\033[2J"; } // clear full screen
inline std::string clear_line() { return "\033[2K"; }   // clear current line
inline std::string clear_line_to_end() {
  return "\033[K";
} // clear from cursor to end of line
inline std::string clear_line_to_start() { return "\033[1K"; }
inline std::string clear_scrollback() { return "\033[3J"; }
inline std::string scroll_up(int n) {
  return "\033[" + std::to_string(n) + "S";
}
inline std::string scroll_down(int n) {
  return "\033[" + std::to_string(n) + "T";
}

// basic colors
inline std::string fg_black() { return "\033[30m"; }
inline std::string fg_red() { return "\033[31m"; }
inline std::string fg_green() { return "\033[32m"; }
inline std::string fg_yellow() { return "\033[33m"; }
inline std::string fg_blue() { return "\033[34m"; }
inline std::string fg_magenta() { return "\033[35m"; }
inline std::string fg_cyan() { return "\033[36m"; }
inline std::string fg_white() { return "\033[37m"; }
inline std::string fg_bright_black() { return "\033[90m"; }
inline std::string fg_bright_red() { return "\033[91m"; }
inline std::string fg_bright_green() { return "\033[92m"; }
inline std::string fg_bright_yellow() { return "\033[93m"; }
inline std::string fg_bright_blue() { return "\033[94m"; }
inline std::string fg_bright_magenta() { return "\033[95m"; }
inline std::string fg_bright_cyan() { return "\033[96m"; }
inline std::string fg_bright_whit() { return "\033[97m"; }

inline std::string bg_black() { return "\033[40m"; }
inline std::string bg_red() { return "\033[41m"; }
inline std::string bg_green() { return "\033[42m"; }
inline std::string bg_yellow() { return "\033[43m"; }
inline std::string bg_blue() { return "\033[44m"; }
inline std::string bg_magenta() { return "\033[45m"; }
inline std::string bg_cyan() { return "\033[46m"; }
inline std::string bg_white() { return "\033[47m"; }
inline std::string bg_bright_black() { return "\033[100m"; }
inline std::string bg_bright_red() { return "\033[101m"; }
inline std::string bg_bright_green() { return "\033[102m"; }
inline std::string bg_bright_yellow() { return "\033[103m"; }
inline std::string bg_bright_blue() { return "\033[104m"; }
inline std::string bg_bright_magenta() { return "\033[105m"; }
inline std::string bg_bright_cyan() { return "\033[106m"; }
inline std::string bg_bright_white() { return "\033[107m"; }

// 8 bit
inline std::string fg_8bit(int n) {
  return "\033[38;5;" + std::to_string(n) + "m";
}
inline std::string bg_8bit(int n) {
  return "\033[48;5;" + std::to_string(n) + "m";
}

// 24 bit
inline std::string fg_rgb(int r, int g, int b) {
  return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
         std::to_string(b) + "m";
}

inline std::string bg_rgb(int r, int g, int b) {
  return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
         std::to_string(b) + "m";
}

// text styles
inline std::string bold() { return "\033[1m"; }
inline std::string underline() { return "\033[4m"; }
inline std::string inverse() { return "\033[7m"; }

inline std::string reset() { return "\033[0m"; }
inline std::string reset_all() { return "\033[2J\033[1;1H"; }

} // namespace vextr::utils::ansi
