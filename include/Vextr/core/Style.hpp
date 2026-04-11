#pragma once
#include <Vextr/core/Align.hpp>
#include <cstdint>

namespace vextr::core {

struct Color {
  uint8_t r = 255, g = 255, b = 255;
  bool transparent = false;

  static Color clear() { return {0, 0, 0, true}; }
};

enum class BorderStyle {
  None,
  Single,  // ─ │ ┌ ┐ └ ┘
  Double,  // ═ ║ ╔ ╗ ╚ ╝
  Rounded, // ─ │ ╭ ╮ ╰ ╯
  Thick,   // ━ ┃ ┏ ┓ ┗ ┛
  Dashed,  // ╌ ╎
};

struct Border {
  BorderStyle style = BorderStyle::None;
  Color color = {255, 255, 255};
};

struct Style {
  Color fg = {255, 255, 255};
  Color bg = {0, 0, 0};
  bool bold = false;
  bool underline = false;

  Border border;

  Align contentAlignX = Align::Start;
  Align contentAlignY = Align::Start;

  int padding = 0;
};

} // namespace vextr::core