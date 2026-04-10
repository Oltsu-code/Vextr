#pragma once
#include <cstdint>

namespace vextr::core {

struct Color {
  uint8_t r = 255, g = 255, b = 255;
  bool transparent = false;

  static Color clear() { return {0, 0, 0, true}; }
};

struct Style {
  Color fg = {255, 255, 255};
  Color bg = {0, 0, 0};
  bool bold = false;
  bool underline = false;

  int padding = 0;
};

} // namespace vextr::core