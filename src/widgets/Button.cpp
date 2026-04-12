#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Button.hpp>

namespace vextr::widgets {

core::Size Button::measure(int availW, int availH) {
  int naturalW = utils::unicode::stringWidth(label) + 4;
  return {naturalW, 1};
}

void Button::drawContent(backend::Buffer &buf, core::Rect inner) {
  const auto &s = activeStyle();
  using namespace vextr::utils::unicode;

  if (inner.width <= 0 || inner.height <= 0)
    return;

  int labelW = 0;
  size_t i = 0;
  while (i < label.size()) {
    uint32_t cp = nextCodepoint(label, i);
    labelW += displayWidth(cp);
  }

  core::Rect r =
      alignContentRect(inner, labelW, 1, s.innerAlignX, s.innerAlignY);

  size_t idx = 0;
  int screenX = r.x;
  int maxX = inner.x + inner.width;

  while (idx < label.size() && screenX < maxX) {
    uint32_t cp = nextCodepoint(label, idx);
    int w = displayWidth(cp);
    if (screenX + w > maxX)
      break;

    backend::Cell cell;
    cell.ch = encode(cp);
    cell.fg = s.fg;
    cell.bg = s.bg;
    cell.bold = s.text.bold;
    cell.underline = s.text.underline;
    buf.set(screenX, r.y, cell);

    if (w == 2 && screenX + 1 < maxX) {
      backend::Cell wide = cell;
      wide.ch = "";
      buf.set(screenX + 1, r.y, wide);
    }

    screenX += w;
  }
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