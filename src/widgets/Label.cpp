#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Label.hpp>

namespace vextr::widgets {

core::Size Label::measure(int availW, int availH) {
  return {utils::unicode::stringWidth(text), 1};
}

void Label::render(backend::Buffer &buf) {
  using namespace vextr::utils::unicode;

  backend::Cell base;
  base.ch = ' ';
  base.fg = style.fg;
  base.bg = style.bg;

  for (int y = rect.y; y < rect.y + rect.height; ++y)
    for (int x = rect.x; x < rect.x + rect.width; ++x)
      buf.set(x, y, base);

  int cx = rect.x;
  int maxX = rect.x + rect.width;

  size_t i = 0;

  while (i < text.size() && cx < maxX) {
    size_t start_i = i;
    uint32_t cp = nextCodepoint(text, i);

    if (i == start_i)
      break;

    int w = displayWidth(cp);

    if (w <= 0)
      continue;

    if (cx + w > maxX)
      break;

    backend::Cell cell;
    cell.ch = encode(cp);

    cell.fg = style.fg;
    cell.bg = style.bg;
    cell.bold = style.bold;
    cell.underline = style.underline;

    buf.set(cx, rect.y, cell);

    if (w == 2 && cx + 1 < maxX) {
      backend::Cell wide_cell = cell;
      wide_cell.ch = "";
      buf.set(cx + 1, rect.y, wide_cell);
    }

    cx += w;
  }
}

} // namespace vextr::widgets