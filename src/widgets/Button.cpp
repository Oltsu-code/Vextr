#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Button.hpp>

namespace vextr::widgets {

core::Size Button::measure(int availW, int availH) {
  return {utils::unicode::stringWidth(label) + 4, 1};
}

void Button::render(backend::Buffer &buf) {
  using namespace vextr::utils::unicode;
  const core::Style &s = activeStyle();

  // fill background
  backend::Cell base;
  base.ch = ' ';
  base.fg = s.fg;
  base.bg = s.bg;
  for (int y = rect.y; y < rect.y + rect.height; ++y)
    for (int x = rect.x; x < rect.x + rect.width; ++x)
      buf.set(x, y, base);

  // draw "[ label ]"
  int cx = rect.x;
  int cy = rect.y + rect.height / 2;
  int maxX = rect.x + rect.width;

  auto putChar = [&](const std::string &ch, int width = 1) {
    if (cx + width > maxX)
      return;
    backend::Cell cell;
    cell.ch = ch;
    cell.fg = s.fg;
    cell.bg = s.bg;
    cell.bold = s.bold;
    cell.underline = s.underline;
    buf.set(cx, cy, cell);

    if (width == 2 && cx + 1 < maxX) {
      backend::Cell wide_cell = cell;
      wide_cell.ch = "";
      buf.set(cx + 1, cy, wide_cell);
    }
    cx += width;
  };

  putChar("[");
  putChar(" ");

  size_t i = 0;
  while (i < label.size() && cx < maxX) {
    size_t start_i = i;
    uint32_t cp = nextCodepoint(label, i);

    if (i == start_i)
      break;

    int w = displayWidth(cp);
    if (w <= 0)
      continue;

    if (cx + w > maxX)
      break;

    putChar(encode(cp), w);
  }

  putChar(" ");
  putChar("]");
}

bool Button::onEvent(const core::Event &e) {
  if (e.type == core::EventType::Key) {
    if (e.key == utils::Key::Enter || e.key == ' ') {
      if (onClick)
        onClick();
      return true;
    }
  }
  return false;
}

} // namespace vextr::widgets