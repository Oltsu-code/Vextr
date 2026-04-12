#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Label.hpp>

namespace vextr::widgets {

core::Size Label::measure(int availW, int availH) {
  return {utils::unicode::stringWidth(text), 1};
}

void Label::drawContent(backend::Buffer &buf, core::Rect inner) {
  const core::Style &s = activeStyle();

  int textW = utils::unicode::stringWidth(text);

  core::Rect r =
      alignContentRect(inner, textW, 1, s.innerAlignX, s.innerAlignY);

  int screenX = r.x;
  int maxX = inner.x + inner.width;
  int cy = r.y;

  size_t i = 0;

  while (i < text.size() && screenX < maxX) {
    size_t start = i;
    uint32_t cp = utils::unicode::nextCodepoint(text, i);
    if (i == start)
      break;

    int w = utils::unicode::displayWidth(cp);
    if (w <= 0)
      continue;
    if (screenX + w > maxX)
      break;

    backend::Cell cell;
    cell.ch = utils::unicode::encode(cp);
    cell.fg = s.fg;
    cell.bg = s.bg;
    cell.bold = s.bold;
    cell.underline = s.underline;

    buf.set(screenX, cy, cell);

    if (w == 2 && screenX + 1 < maxX) {
      backend::Cell wide;
      wide.ch = "";
      wide.fg = s.fg;
      wide.bg = s.bg;
      buf.set(screenX + 1, cy, wide);
    }

    screenX += w;
  }
}

} // namespace vextr::widgets