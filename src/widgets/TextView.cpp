#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/TextFlow.hpp>
#include <Vextr/widgets/TextView.hpp>
#include <algorithm>

namespace vextr::widgets {

namespace {

struct ViewportState {
  std::vector<detail::VisualLine> rows;
  int contentWidth = 0;
  int textWidth = 0;
  int textHeight = 0;
  int maxScrollX = 0;
  int maxScrollY = 0;
  bool showVScrollbar = false;
  bool showHScrollbar = false;
};

constexpr const char *kVScrollbarTrack = "│";
constexpr const char *kHScrollbarTrack = "─";
constexpr const char *kScrollbarThumb = "█";
constexpr const char *kScrollbarCorner = "┼";

ViewportState layoutState(const std::string &text, TextFlowMode mode,
                          core::Rect inner) {
  ViewportState state;
  if (inner.width <= 0 || inner.height <= 0) {
    return state;
  }

  auto compute = [&](bool showV, bool showH) {
    const int textWidth = std::max(1, inner.width - (showV ? 1 : 0));
    const int textHeight = std::max(1, inner.height - (showH ? 1 : 0));
    const int wrapWidth = mode == TextFlowMode::Wrap ? textWidth : 1;
    state.rows = detail::buildVisualLines(text, mode, wrapWidth);
    state.contentWidth = 0;
    for (const auto &row : state.rows) {
      state.contentWidth = std::max(state.contentWidth, row.width);
    }
    state.textWidth = textWidth;
    state.textHeight = textHeight;
    state.maxScrollX = std::max(0, state.contentWidth - state.textWidth);
    state.maxScrollY =
        std::max(0, static_cast<int>(state.rows.size()) - state.textHeight);
  };

  bool showV = false;
  bool showH = false;
  for (int pass = 0; pass < 4; ++pass) {
    compute(showV, showH);
    const bool wantV = state.maxScrollY > 0 && inner.width > 1;
    const bool wantH = state.maxScrollX > 0 && inner.height > 1;
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

void TextView::setScrollX(int x) { scrollOffsetX = std::max(0, x); }

void TextView::setScrollY(int y) { scrollOffsetY = std::max(0, y); }

bool TextView::isFocusable() const {
  const ViewportState state = layoutState(text, flowMode, innerRect());
  return state.maxScrollX > 0 || state.maxScrollY > 0;
}

bool TextView::onEvent(const core::Event &event) {
  if (event.type != core::EventType::Key) {
    return false;
  }

  const core::Rect inner = innerRect();
  const ViewportState state = layoutState(text, flowMode, inner);
  if (state.maxScrollX <= 0 && state.maxScrollY <= 0) {
    return false;
  }

  switch (event.key) {
  case utils::Key::Up:
    if (state.maxScrollY <= 0)
      return false;
    setScrollY(std::clamp(scrollOffsetY - 1, 0, state.maxScrollY));
    return true;
  case utils::Key::Down:
    if (state.maxScrollY <= 0)
      return false;
    setScrollY(std::clamp(scrollOffsetY + 1, 0, state.maxScrollY));
    return true;
  case utils::Key::PageUp:
    if (state.maxScrollY <= 0)
      return false;
    setScrollY(std::clamp(scrollOffsetY - std::max(1, state.textHeight), 0,
                          state.maxScrollY));
    return true;
  case utils::Key::PageDown:
    if (state.maxScrollY <= 0)
      return false;
    setScrollY(std::clamp(scrollOffsetY + std::max(1, state.textHeight), 0,
                          state.maxScrollY));
    return true;
  case utils::Key::Left:
    if (state.maxScrollX <= 0)
      return false;
    setScrollX(std::clamp(scrollOffsetX - 1, 0, state.maxScrollX));
    return true;
  case utils::Key::Right:
    if (state.maxScrollX <= 0)
      return false;
    setScrollX(std::clamp(scrollOffsetX + 1, 0, state.maxScrollX));
    return true;
  case utils::Key::Home:
    setScrollX(0);
    setScrollY(0);
    return true;
  case utils::Key::End:
    setScrollX(state.maxScrollX);
    setScrollY(state.maxScrollY);
    return true;
  default:
    return false;
  }
}

core::Size TextView::measure(int availW, int availH) {
  const int widthLimit = std::max(1, availW);
  const int heightLimit = std::max(1, availH);
  const ViewportState state =
      layoutState(text, flowMode, {0, 0, widthLimit, heightLimit});

  const int measuredWidth = state.contentWidth + (state.showVScrollbar ? 1 : 0);
  const int measuredHeight =
      static_cast<int>(state.rows.size()) + (state.showHScrollbar ? 1 : 0);
  return {std::min(measuredWidth, widthLimit),
          std::min(measuredHeight, heightLimit)};
}

void TextView::drawContent(backend::Buffer &buf, core::Rect inner) {
  if (inner.width <= 0 || inner.height <= 0) {
    return;
  }

  const core::Style &s = activeStyle();
  const ViewportState state = layoutState(text, flowMode, inner);
  const auto &rows = state.rows;
  const int scrollX = std::clamp(scrollOffsetX, 0, state.maxScrollX);
  const int scrollY = std::clamp(scrollOffsetY, 0, state.maxScrollY);
  const bool showVScrollbar = state.showVScrollbar;
  const bool showHScrollbar = state.showHScrollbar;
  const int rightEdge = inner.x + state.textWidth;
  const int bottomEdge = inner.y + state.textHeight;

  const int vThumbSize =
      showVScrollbar
          ? std::max(1, (state.textHeight * state.textHeight) /
                            std::max(1, static_cast<int>(rows.size())))
          : 0;
  const int vThumbTravel =
      showVScrollbar ? std::max(0, state.textHeight - vThumbSize) : 0;
  const int vThumbTop = (showVScrollbar && state.maxScrollY > 0)
                            ? (scrollY * vThumbTravel) / state.maxScrollY
                            : 0;

  const int hThumbSize = showHScrollbar
                             ? std::max(1, (state.textWidth * state.textWidth) /
                                               std::max(1, state.contentWidth))
                             : 0;
  const int hThumbTravel =
      showHScrollbar ? std::max(0, state.textWidth - hThumbSize) : 0;
  const int hThumbLeft = (showHScrollbar && state.maxScrollX > 0)
                             ? (scrollX * hThumbTravel) / state.maxScrollX
                             : 0;

  int firstLine = std::clamp(scrollY, 0, static_cast<int>(rows.size()));
  int lastLine =
      std::min(static_cast<int>(rows.size()), firstLine + state.textHeight);

  for (int rowIndex = firstLine; rowIndex < lastLine; ++rowIndex) {
    const auto &row = rows[static_cast<std::size_t>(rowIndex)];
    std::string line(text.substr(row.startByte, row.endByte - row.startByte));
    int contentX = 0;
    int y = inner.y + (rowIndex - firstLine);
    std::size_t i = 0;

    while (i < line.size()) {
      std::size_t start = i;
      uint32_t cp = utils::unicode::nextCodepoint(line, i);
      if (i == start) {
        break;
      }

      int w = utils::unicode::displayWidth(cp);
      if (w <= 0) {
        continue;
      }
      if (contentX + w <= scrollX) {
        contentX += w;
        continue;
      }
      if (contentX < scrollX) {
        contentX += w;
        continue;
      }

      int screenX = inner.x + (contentX - scrollX);
      if (screenX >= rightEdge || screenX + w > rightEdge) {
        break;
      }

      backend::Cell cell;
      cell.ch = utils::unicode::encode(cp);
      cell.fg = s.fg;
      cell.bg = s.bg;
      cell.bold = s.text.bold;
      cell.underline = s.text.underline;
      buf.set(screenX, y, cell);

      if (w == 2 && screenX + 1 < rightEdge) {
        backend::Cell wide;
        wide.ch = "";
        wide.fg = s.fg;
        wide.bg = s.bg;
        buf.set(screenX + 1, y, wide);
      }

      contentX += w;
    }
  }

  if (showVScrollbar) {
    for (int row = 0; row < state.textHeight; ++row) {
      const bool inThumb = row >= vThumbTop && row < vThumbTop + vThumbSize;
      backend::Cell indicator;
      indicator.ch = inThumb ? kScrollbarThumb : kVScrollbarTrack;
      indicator.fg = s.fg;
      indicator.bg = s.bg;
      indicator.bold = s.text.bold;
      indicator.underline = s.text.underline;
      buf.set(inner.x + state.textWidth, inner.y + row, indicator);
    }
  }

  if (showHScrollbar) {
    for (int col = 0; col < state.textWidth; ++col) {
      const bool inThumb = col >= hThumbLeft && col < hThumbLeft + hThumbSize;
      backend::Cell indicator;
      indicator.ch = inThumb ? kScrollbarThumb : kHScrollbarTrack;
      indicator.fg = s.fg;
      indicator.bg = s.bg;
      indicator.bold = s.text.bold;
      indicator.underline = s.text.underline;
      buf.set(inner.x + col, bottomEdge, indicator);
    }
  }

  if (showVScrollbar && showHScrollbar) {
    backend::Cell corner;
    corner.ch = kScrollbarCorner;
    corner.fg = s.fg;
    corner.bg = s.bg;
    corner.bold = s.text.bold;
    corner.underline = s.text.underline;
    buf.set(inner.x + state.textWidth, bottomEdge, corner);
  }
}

} // namespace vextr::widgets
