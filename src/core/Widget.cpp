#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Context.hpp>
#include <Vextr/core/Widget.hpp>
#include <algorithm>

namespace vextr::core {

// border character sets
struct BorderChars {
  const char *h;  // horizontal
  const char *v;  // vertical
  const char *tl; // top left
  const char *tr; // top right
  const char *bl; // bottom left
  const char *br; // bottom right
};

static const BorderChars borderSets[] = {
    // none
    {},
    // single  ─ │ ┌ ┐ └ ┘
    {"\xe2\x94\x80", "\xe2\x94\x82", "\xe2\x94\x8c", "\xe2\x94\x90",
     "\xe2\x94\x94", "\xe2\x94\x98"},
    // double  ═ ║ ╔ ╗ ╚ ╝
    {"\xe2\x95\x90", "\xe2\x95\x91", "\xe2\x95\x94", "\xe2\x95\x97",
     "\xe2\x95\x9a", "\xe2\x95\x9d"},
    // rounded ─ │ ╭ ╮ ╰ ╯
    {"\xe2\x94\x80", "\xe2\x94\x82", "\xe2\x95\xad", "\xe2\x95\xae",
     "\xe2\x95\xb0", "\xe2\x95\xaf"},
    // thick   ━ ┃ ┏ ┓ ┗ ┛
    {"\xe2\x94\x81", "\xe2\x94\x83", "\xe2\x94\x8f", "\xe2\x94\x93",
     "\xe2\x94\x97", "\xe2\x94\x9b"},
    // dashed  ╌ ╎
    {"\xe2\x95\x8c", "\xe2\x95\x8e", "\xe2\x94\x8c", "\xe2\x94\x90",
     "\xe2\x94\x94", "\xe2\x94\x98"},
};

bool Widget::focused() const {
  return Context::get().focusManager.focused().get() == this;
}

const Style &Widget::activeStyle() const {
  return focused() ? focusedStyle : style;
}

Rect Widget::innerRect() const {
  const Style &s = style;
  int b = (s.border.style != BorderStyle::None) ? 1 : 0;
  return {
      rect.x + b + s.padding.left,
      rect.y + b + s.padding.top,
      std::max(0, rect.width - b * 2 - s.padding.left - s.padding.right),
      std::max(0, rect.height - b * 2 - s.padding.top - s.padding.bottom),
  };
}

Size Widget::measure(int availW, int availH) { return {0, 0}; }

void Widget::layout(int x, int y, int width, int height) {
  rect = {x, y, width, height};
}

void Widget::render(backend::Buffer &buf) {
  drawBackground(buf);
  drawBorder(buf);
  drawContent(buf, innerRect());
}

void Widget::drawBackground(backend::Buffer &buf) {
  const Style &s = activeStyle();
  if (s.bg.transparent)
    return;

  backend::Cell cell;
  cell.ch = ' ';
  cell.fg = s.fg;
  cell.bg = s.bg;
  for (int y = rect.y; y < rect.y + rect.height; ++y)
    for (int x = rect.x; x < rect.x + rect.width; ++x)
      buf.set(x, y, cell);
}

void Widget::drawBorder(backend::Buffer &buf) {
  const Style &s = activeStyle();
  if (s.border.style == BorderStyle::None)
    return;
  if (rect.width < 2 || rect.height < 2)
    return;

  int idx = (int)s.border.style;
  const BorderChars &bc = borderSets[idx];

  auto put = [&](int x, int y, const char *ch) {
    if (!ch || !ch[0])
      return;
    backend::Cell cell;
    cell.ch = std::string(ch);
    cell.fg = s.border.color;
    cell.bg = s.bg;
    buf.set(x, y, cell);
  };

  int x0 = rect.x, y0 = rect.y;
  int x1 = rect.x + rect.width - 1;
  int y1 = rect.y + rect.height - 1;

  put(x0, y0, bc.tl);
  put(x1, y0, bc.tr);
  put(x0, y1, bc.bl);
  put(x1, y1, bc.br);
  for (int x = x0 + 1; x < x1; ++x) {
    put(x, y0, bc.h);
    put(x, y1, bc.h);
  }
  for (int y = y0 + 1; y < y1; ++y) {
    put(x0, y, bc.v);
    put(x1, y, bc.v);
  }
}

int Widget::justifyX(int innerX, int innerW, int contentW, Align align) const {
  switch (align) {
  case Align::Center:
    return innerX + std::max(0, (innerW - contentW) / 2);
  case Align::End:
    return innerX + std::max(0, innerW - contentW);
  default:
    return innerX;
  }
}

int Widget::justifyY(int innerY, int innerH, int contentH, Align align) const {
  switch (align) {
  case Align::Center:
    return innerY + std::max(0, (innerH - contentH) / 2);
  case Align::End:
    return innerY + std::max(0, innerH - contentH);
  default:
    return innerY;
  }
}

Rect Widget::alignContentRect(Rect inner, int contentW, int contentH, Align ax,
                              Align ay) const {
  int x = justifyX(inner.x, inner.width, contentW, ax);
  int y = justifyY(inner.y, inner.height, contentH, ay);
  return {x, y, contentW, contentH};
}

} // namespace vextr::core