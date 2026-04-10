#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Label.hpp>

namespace vextr::widgets {

core::Size Label::measure(int availW, int availH) {
  return {utils::unicode::stringWidth(text), 1};
}

void Label::render(backend::Buffer &buf) {
  backend::Cell base;
  base.ch = ' ';
  base.fg = style.fg;
  base.bg = style.bg;
  for (int y = rect.y; y < rect.y + rect.height; ++y)
    for (int x = rect.x; x < rect.x + rect.width; ++x)
      buf.set(x, y, base);

  int cx = rect.x;
  int maxX = rect.x + rect.width;
  for (unsigned char c : text) {
    if (cx >= maxX)
      break;
    backend::Cell cell;
    cell.ch = c;
    cell.fg = style.fg;
    cell.bg = style.bg;
    cell.bold = style.bold;
    cell.underline = style.underline;
    buf.set(cx, rect.y, cell);
    cx++;
  }
}

} // namespace vextr::widgets