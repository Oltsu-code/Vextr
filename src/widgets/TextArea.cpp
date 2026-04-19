#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/TextArea.hpp>
#include <Vextr/widgets/TextFlow.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <algorithm>

namespace vextr::widgets {

namespace {

constexpr const char *kVScrollbarTrack = "│";
constexpr const char *kHScrollbarTrack = "─";
constexpr const char *kScrollbarThumb = "█";
constexpr const char *kScrollbarCorner = "┼";

struct TextAreaViewportState {
  std::vector<detail::VisualLine> rows;
  int contentWidth = 0;
  int textWidth = 0;
  int textHeight = 0;
  int maxScrollX = 0;
  int maxScrollY = 0;
  bool showVScrollbar = false;
  bool showHScrollbar = false;
};

TextAreaViewportState viewportState(const std::string &text,
                                    TextFlowMode flowMode, core::Rect inner) {
  TextAreaViewportState state;
  if (inner.width <= 0 || inner.height <= 0) {
    return state;
  }

  auto compute = [&](bool showV, bool showH) {
    state.textWidth = std::max(1, inner.width - (showV ? 1 : 0));
    state.textHeight = std::max(1, inner.height - (showH ? 1 : 0));
    state.rows = detail::buildVisualLines(
        text, flowMode, flowMode == TextFlowMode::Wrap ? state.textWidth : 1);

    state.contentWidth = 0;
    for (const auto &row : state.rows) {
      state.contentWidth = std::max(state.contentWidth, row.width);
    }

    state.maxScrollX = std::max(0, state.contentWidth - state.textWidth);
    state.maxScrollY =
        std::max(0, static_cast<int>(state.rows.size()) - state.textHeight);
  };

  bool showV = false;
  bool showH = false;
  for (int pass = 0; pass < 4; ++pass) {
    compute(showV, showH);
    const bool wantH = flowMode == TextFlowMode::Scroll &&
                       state.maxScrollX > 0 && inner.height > 1;
    const bool wantV = state.maxScrollY > 0 && inner.width > 1;
    if (wantV == showV && wantH == showH) {
      break;
    }
    showV = wantV;
    showH = wantH;
  }

  compute(showV, showH);
  state.showVScrollbar = showV;
  state.showHScrollbar = showH;

  return state;
}

} // namespace

void TextArea::setText(const std::string &t) {
  text = t;
  cursor = static_cast<int>(text.size());
  preferredColumn = -1;
  updateScroll();
}

core::Size TextArea::measure(int availW, int availH) {
  const int widthLimit = std::max(1, availW);
  const int heightLimit = std::max(1, availH);
  const auto state =
      viewportState(text, flowMode, {0, 0, widthLimit, heightLimit});

  const int measuredWidth = state.contentWidth + (state.showVScrollbar ? 1 : 0);
  const int measuredHeight =
      static_cast<int>(state.rows.size()) + (state.showHScrollbar ? 1 : 0);
  return {std::min(measuredWidth, widthLimit),
          std::min(measuredHeight, heightLimit)};
}

void TextArea::insertChar(char c) {
  cursor = std::clamp(cursor, 0, static_cast<int>(text.size()));
  text.insert(text.begin() + cursor, c);
  ++cursor;
  preferredColumn = -1;
  updateScroll();
  if (onChange) {
    onChange(text);
  }
}

void TextArea::insertNewline() { insertChar('\n'); }

void TextArea::deleteBack() {
  if (cursor <= 0 || text.empty()) {
    return;
  }

  text.erase(text.begin() + cursor - 1);
  --cursor;
  preferredColumn = -1;
  updateScroll();
  if (onChange) {
    onChange(text);
  }
}

void TextArea::deleteForward() {
  if (cursor >= static_cast<int>(text.size())) {
    return;
  }

  text.erase(text.begin() + cursor);
  preferredColumn = -1;
  updateScroll();
  if (onChange) {
    onChange(text);
  }
}

void TextArea::moveCursorHorizontal(int delta) {
  cursor = std::clamp(cursor + delta, 0, static_cast<int>(text.size()));
  preferredColumn = -1;
  updateScroll();
}

void TextArea::moveCursorVertical(int delta) {
  const core::Rect inner = innerRect();
  const auto state = viewportState(text, flowMode, inner);
  const int wrapWidth = std::max(1, state.textWidth);
  const auto location = detail::locateCursor(
      text, cursor, flowMode, flowMode == TextFlowMode::Wrap ? wrapWidth : 1);

  if (preferredColumn < 0) {
    preferredColumn = location.column;
  }

  const auto rows = detail::buildVisualLines(
      text, flowMode, flowMode == TextFlowMode::Wrap ? wrapWidth : 1);
  int targetRow =
      std::clamp(location.row + delta, 0, static_cast<int>(rows.size()) - 1);

  cursor = detail::cursorForRowColumn(
      text, targetRow, preferredColumn, flowMode,
      flowMode == TextFlowMode::Wrap ? wrapWidth : 1);
  updateScroll();
}

void TextArea::moveToLineBoundary(bool endOfLine) {
  const core::Rect inner = innerRect();
  const auto state = viewportState(text, flowMode, inner);
  const int wrapWidth = std::max(1, state.textWidth);
  const auto location = detail::locateCursor(
      text, cursor, flowMode, flowMode == TextFlowMode::Wrap ? wrapWidth : 1);

  if (endOfLine) {
    cursor = static_cast<int>(location.rowEnd);
  } else {
    cursor = static_cast<int>(location.rowStart);
  }

  preferredColumn = -1;
  updateScroll();
}

void TextArea::updateScroll() {
  const core::Rect inner = innerRect();
  if (inner.width <= 0 || inner.height <= 0) {
    return;
  }

  const auto state = viewportState(text, flowMode, inner);

  const auto location = detail::locateCursor(
      text, cursor, flowMode,
      flowMode == TextFlowMode::Wrap ? state.textWidth : 1);

  if (flowMode == TextFlowMode::Scroll) {
    if (location.column < scrollX) {
      scrollX = location.column;
    }
    if (location.column >= scrollX + state.textWidth) {
      scrollX = location.column - state.textWidth + 1;
    }
  } else {
    scrollX = 0;
  }

  if (location.row < scrollY) {
    scrollY = location.row;
  }
  if (location.row >= scrollY + state.textHeight) {
    scrollY = location.row - state.textHeight + 1;
  }

  scrollX = std::clamp(scrollX, 0, state.maxScrollX);
  scrollY = std::clamp(scrollY, 0, state.maxScrollY);
}

void TextArea::drawContent(backend::Buffer &buf, core::Rect inner) {
  if (inner.width <= 0 || inner.height <= 0) {
    return;
  }

  const core::Style &s = activeStyle();
  const auto state = viewportState(text, flowMode, inner);
  const auto &rows = state.rows;
  const int clampedScrollX = std::clamp(scrollX, 0, state.maxScrollX);
  const int clampedScrollY = std::clamp(scrollY, 0, state.maxScrollY);
  const int rightEdge = inner.x + state.textWidth;
  const int bottomEdge = inner.y + state.textHeight;

  bool showPlaceholder = text.empty() && !placeholder.empty();
  const bool wrapMode = flowMode == TextFlowMode::Wrap;
  for (int row = 0; row < state.textHeight; ++row) {
    int y = inner.y + row;
    int lineIndex = clampedScrollY + row;
    if (lineIndex >= static_cast<int>(rows.size())) {
      break;
    }

    std::string lineText;
    if (showPlaceholder && lineIndex == 0) {
      lineText = placeholder;
    } else {
      const auto &visualLine = rows[static_cast<std::size_t>(lineIndex)];
      lineText = text.substr(visualLine.startByte,
                             visualLine.endByte - visualLine.startByte);
    }

    int screenX = inner.x;
    int textColumn = 0;
    std::size_t i = 0;
    while (i < lineText.size() && screenX < rightEdge) {
      std::size_t start = i;
      uint32_t cp = utils::unicode::nextCodepoint(lineText, i);
      if (i == start) {
        break;
      }

      int w = utils::unicode::displayWidth(cp);
      if (w <= 0) {
        continue;
      }
      if (!wrapMode && textColumn + w <= clampedScrollX) {
        textColumn += w;
        continue;
      }
      if (screenX + w > rightEdge) {
        break;
      }

      backend::Cell cell;
      cell.ch = utils::unicode::encode(cp);
      if (showPlaceholder) {
        cell.fg = {(uint8_t)(s.fg.r / 2), (uint8_t)(s.fg.g / 2),
                   (uint8_t)(s.fg.b / 2)};
      } else {
        cell.fg = s.fg;
      }
      cell.bg = s.bg;
      cell.bold = s.text.bold;
      cell.underline = s.text.underline;
      buf.set(screenX, y, cell);

      if (w == 2 && screenX + 1 < rightEdge) {
        backend::Cell wide;
        wide.ch = "";
        wide.fg = cell.fg;
        wide.bg = cell.bg;
        wide.bold = cell.bold;
        wide.underline = cell.underline;
        buf.set(screenX + 1, y, wide);
      }

      textColumn += w;
      screenX += w;
    }
  }

  if (focused()) {
    const auto location = detail::locateCursor(
        text, cursor, flowMode,
        flowMode == TextFlowMode::Wrap ? state.textWidth : 1);

    int x = inner.x + (location.column - clampedScrollX);
    int y = inner.y + (location.row - clampedScrollY);
    if (x >= inner.x && x < rightEdge && y >= inner.y && y < bottomEdge) {
      backend::Cell cursorCell;
      cursorCell.ch = " ";
      if (cursor < static_cast<int>(text.size()) &&
          text[static_cast<size_t>(cursor)] != '\n') {
        cursorCell.ch = std::string(1, text[static_cast<size_t>(cursor)]);
      }
      cursorCell.fg = s.bg;
      cursorCell.bg = s.fg;
      cursorCell.bold = s.text.bold;
      cursorCell.underline = s.text.underline;
      buf.set(x, y, cursorCell);
    }
  }

  if (state.showHScrollbar) {
    const int thumbSize = std::max(1, (state.textWidth * state.textWidth) /
                                          std::max(1, state.contentWidth));
    const int thumbTravel = std::max(0, state.textWidth - thumbSize);
    const int thumbLeft =
        state.maxScrollX > 0 ? (clampedScrollX * thumbTravel) / state.maxScrollX
                             : 0;
    const int y = bottomEdge;

    for (int x = 0; x < state.textWidth; ++x) {
      const bool inThumb = x >= thumbLeft && x < thumbLeft + thumbSize;
      backend::Cell bar;
      bar.ch = inThumb ? kScrollbarThumb : kHScrollbarTrack;
      bar.fg = s.fg;
      bar.bg = s.bg;
      bar.bold = s.text.bold;
      bar.underline = s.text.underline;
      buf.set(inner.x + x, y, bar);
    }
  }

  if (state.showVScrollbar) {
    const int thumbSize =
        std::max(1, (state.textHeight * state.textHeight) /
                        std::max(1, static_cast<int>(rows.size())));
    const int thumbTravel = std::max(0, state.textHeight - thumbSize);
    const int thumbTop = state.maxScrollY > 0
                             ? (clampedScrollY * thumbTravel) / state.maxScrollY
                             : 0;

    for (int y = 0; y < state.textHeight; ++y) {
      const bool inThumb = y >= thumbTop && y < thumbTop + thumbSize;
      backend::Cell bar;
      bar.ch = inThumb ? kScrollbarThumb : kVScrollbarTrack;
      bar.fg = s.fg;
      bar.bg = s.bg;
      bar.bold = s.text.bold;
      bar.underline = s.text.underline;
      buf.set(inner.x + state.textWidth, inner.y + y, bar);
    }
  }

  if (state.showHScrollbar && state.showVScrollbar) {
    backend::Cell corner;
    corner.ch = kScrollbarCorner;
    corner.fg = s.fg;
    corner.bg = s.bg;
    corner.bold = s.text.bold;
    corner.underline = s.text.underline;
    buf.set(inner.x + state.textWidth, bottomEdge, corner);
  }
}

bool TextArea::onEvent(const core::Event &e) {
  if (e.type != core::EventType::Key) {
    return false;
  }

  switch (e.key) {
  case utils::Key::Left:
    moveCursorHorizontal(-1);
    return true;
  case utils::Key::Right:
    moveCursorHorizontal(1);
    return true;
  case utils::Key::Up:
    moveCursorVertical(-1);
    return true;
  case utils::Key::Down:
    moveCursorVertical(1);
    return true;
  case utils::Key::Home:
    moveToLineBoundary(false);
    return true;
  case utils::Key::End:
    moveToLineBoundary(true);
    return true;
  case utils::Key::Backspace:
    deleteBack();
    return true;
  case utils::Key::Delete:
    deleteForward();
    return true;
  case utils::Key::Enter:
    insertNewline();
    return true;
  case utils::Key::Escape:
    return false;
  default:
    if (e.key >= 32 && e.key < 127 && !e.ctrl && !e.alt) {
      insertChar(static_cast<char>(e.key));
      return true;
    }
    return false;
  }
}

} // namespace vextr::widgets
