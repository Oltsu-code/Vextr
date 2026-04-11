#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Input.hpp>
#include <algorithm>

namespace vextr::widgets {

core::Size Input::measure(int availW, int availH) { return {availW, 1}; }

void Input::updateScroll() {
  int visibleW = rect.width - 2;
  if (visibleW <= 0)
    return;
  if (cursor - scroll >= visibleW)
    scroll = cursor - visibleW + 1;
  if (cursor - scroll < 0)
    scroll = cursor;
  scroll = std::max(0, scroll);
}

void Input::insertChar(char c) {
  if (cursor > (int)text.size())
    cursor = (int)text.size();
  text.insert(text.begin() + cursor, c);
  cursor++;
  updateScroll();
  if (onChange)
    onChange(text);
}

void Input::deleteBack() {
  if (cursor <= 0 || text.empty())
    return;
  text.erase(text.begin() + cursor - 1);
  cursor--;
  updateScroll();
  if (onChange)
    onChange(text);
}

void Input::deleteForward() {
  if (cursor >= (int)text.size())
    return;
  text.erase(text.begin() + cursor);
  updateScroll();
  if (onChange)
    onChange(text);
}

void Input::moveCursor(int delta) {
  cursor = std::clamp(cursor + delta, 0, (int)text.size());
  updateScroll();
}

void Input::drawContent(backend::Buffer &buf, core::Rect inner) {
  const core::Style &s = activeStyle();

  int visibleW = inner.width;
  int cy = inner.y + inner.height / 2;
  bool isEmpty = text.empty();
  bool isFocused = focused();

  const std::string &display = isEmpty ? placeholder : text;

  // render visible characters
  int screenX = inner.x;
  int maxX = inner.x + inner.width;
  size_t i = 0;
  int byteIdx = 0;

  // skip scrolled characters
  size_t tempI = 0;
  int skipped = 0;
  while (skipped < scroll && tempI < display.size()) {
    size_t before = tempI;
    utils::unicode::nextCodepoint(display, tempI);
    if (tempI == before)
      break;
    skipped++;
  }
  i = tempI;

  while (i < display.size() && screenX < maxX) {
    size_t start = i;
    uint32_t cp = utils::unicode::nextCodepoint(display, i);
    if (i == start)
      break;

    int w = utils::unicode::displayWidth(cp);
    if (w <= 0)
      continue;
    if (screenX + w > maxX)
      break;

    backend::Cell cell;
    cell.ch = utils::unicode::encode(cp);
    if (isEmpty) {
      cell.fg = {(uint8_t)(s.fg.r / 2), (uint8_t)(s.fg.g / 2),
                 (uint8_t)(s.fg.b / 2)};
    } else {
      cell.fg = s.fg;
    }
    cell.bg = s.bg;
    cell.bold = s.bold;
    cell.underline = s.underline;
    buf.set(screenX, cy, cell);

    if (w == 2 && screenX + 1 < maxX) {
      backend::Cell wide;
      wide.ch = "";
      wide.fg = cell.fg;
      wide.bg = s.bg;
      buf.set(screenX + 1, cy, wide);
    }

    screenX += w;
  }

  // cursor
  if (isFocused) {
    int cursorScreenX = inner.x + (cursor - scroll);
    if (cursorScreenX >= inner.x && cursorScreenX < maxX) {
      backend::Cell cursorCell;
      cursorCell.ch =
          (cursor < (int)text.size()) ? std::string(1, text[cursor]) : " ";
      cursorCell.fg = s.bg;
      cursorCell.bg = s.fg;
      buf.set(cursorScreenX, cy, cursorCell);
    }
  }
}

bool Input::onEvent(const core::Event &e) {
  if (e.type != core::EventType::Key)
    return false;

  switch (e.key) {
  case utils::Key::Left:
    moveCursor(-1);
    return true;
  case utils::Key::Right:
    moveCursor(1);
    return true;
  case utils::Key::Home:
    cursor = 0;
    updateScroll();
    return true;
  case utils::Key::End:
    cursor = (int)text.size();
    updateScroll();
    return true;
  case utils::Key::Backspace:
    deleteBack();
    return true;
  case utils::Key::Delete:
    deleteForward();
    return true;
  case utils::Key::Enter:
    if (onSubmit)
      onSubmit(text);
    return true;
  case utils::Key::Escape:
    return false;
  default:
    if (e.key >= 32 && e.key < 127 && !e.ctrl && !e.alt) {
      insertChar((char)e.key);
      return true;
    }
    return false;
  }
}

} // namespace vextr::widgets