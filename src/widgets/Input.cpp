#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/Input.hpp>
#include <algorithm>

namespace vextr::widgets {

core::Size Input::measure(int availW, int availH) { return {availW, 1}; }

void Input::updateScroll() {
  int visibleW = rect.width - 2; // 1 char padding each side
  if (visibleW <= 0)
    return;

  // keep cursor visible
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

void Input::render(backend::Buffer &buf) {
  const core::Style &s = activeStyle();
  int visibleW = rect.width - 2;
  int cy = rect.y + rect.height / 2;

  // fill background
  backend::Cell base;
  base.ch = ' ';
  base.fg = s.fg;
  base.bg = s.bg;
  for (int y = rect.y; y < rect.y + rect.height; ++y)
    for (int x = rect.x; x < rect.x + rect.width; ++x)
      buf.set(x, y, base);

  // left padding space
  buf.set(rect.x, cy, base);

  bool isEmpty = text.empty();
  bool isFocused = focused();

  // draw text or placeholder
  const std::string &display = isEmpty ? placeholder : text;
  int startX = rect.x + 1;
  int maxX = rect.x + rect.width - 1;

  for (int i = 0; i < visibleW && (scroll + i) < (int)display.size(); ++i) {
    backend::Cell cell;
    cell.ch = display[scroll + i];
    if (isEmpty) {
      // placeholder style — dimmer
      cell.fg = {(uint8_t)(s.fg.r / 2), (uint8_t)(s.fg.g / 2),
                 (uint8_t)(s.fg.b / 2)};
    } else {
      cell.fg = s.fg;
    }
    cell.bg = s.bg;
    cell.bold = s.bold;
    cell.underline = s.underline;
    buf.set(startX + i, cy, cell);
  }

  // draw cursor when focused
  if (isFocused) {
    int cursorScreenX = startX + (cursor - scroll);
    if (cursorScreenX >= startX && cursorScreenX < maxX) {
      backend::Cell cursorCell;
      // cursor: invert colors
      cursorCell.ch = (cursor < (int)text.size()) ? text[cursor] : ' ';
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
    return false; // let escape bubble
  default:
    // printable characters
    if (e.key >= 32 && e.key < 127 && !e.ctrl && !e.alt) {
      insertChar((char)e.key);
      return true;
    }
    return false;
  }
}

} // namespace vextr::widgets