#include <Vextr/core/Context.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <Vextr/widgets/Dropdown.hpp>
#include <algorithm>

namespace vextr::widgets {

Dropdown::Dropdown(std::vector<Option> options) : options(std::move(options)) {}

core::Size Dropdown::measure(int availW, int availH) {
  if (options.empty()) {
    return {11, 1};
  }
  // find longest label
  int maxWidth = 0;
  for (const auto &opt : options) {
    int w = utils::unicode::stringWidth(opt.label);
    maxWidth = std::max(maxWidth, w);
  }
  // add space for arrow
  int arrowWidth = utils::unicode::stringWidth(" ▼ ");
  return {std::min(availW, maxWidth + arrowWidth), 1};
}

void Dropdown::drawContent(backend::Buffer &buf, core::Rect inner) {
  const auto &s = activeStyle();
  std::string label =
      options.empty() ? "(no options)" : options[selected].label;
  std::string arrow = open ? " ▲ " : " ▼ ";

  int y = inner.y + inner.height / 2;
  int arrowWidth = utils::unicode::stringWidth(arrow);
  int maxLabelWidth = inner.width - arrowWidth;

  // draw label
  int x = inner.x;
  size_t i = 0;
  int drawnWidth = 0;

  while (i < label.size() && drawnWidth < maxLabelWidth) {
    size_t start = i;
    uint32_t cp = utils::unicode::nextCodepoint(label, i);
    if (i == start)
      break;

    int w = utils::unicode::displayWidth(cp);
    if (w <= 0)
      continue;
    if (drawnWidth + w > maxLabelWidth)
      break;

    backend::Cell cell;
    cell.ch = utils::unicode::encode(cp);
    cell.fg = s.fg;
    cell.bg = s.bg;
    cell.bold = s.text.bold;
    buf.set(x, y, cell);

    if (w == 2 && x + 1 < inner.x + inner.width) {
      backend::Cell wide;
      wide.ch = "";
      wide.fg = s.fg;
      wide.bg = s.bg;
      buf.set(x + 1, y, wide);
    }

    x += w;
    drawnWidth += w;
  }

  // draw arrow
  x = inner.x + inner.width - arrowWidth;
  i = 0;
  while (i < arrow.size() && x < inner.x + inner.width) {
    size_t start = i;
    uint32_t cp = utils::unicode::nextCodepoint(arrow, i);
    if (i == start)
      break;

    int w = utils::unicode::displayWidth(cp);
    if (w <= 0)
      continue;
    if (x + w > inner.x + inner.width)
      break;

    backend::Cell cell;
    cell.ch = utils::unicode::encode(cp);
    cell.fg = s.fg;
    cell.bg = s.bg;
    cell.bold = s.text.bold;
    buf.set(x, y, cell);

    if (w == 2 && x + 1 < inner.x + inner.width) {
      backend::Cell wide;
      wide.ch = "";
      wide.fg = s.fg;
      wide.bg = s.bg;
      buf.set(x + 1, y, wide);
    }

    x += w;
  }
}

bool Dropdown::onEvent(const core::Event &e) {
  if (e.type != core::EventType::Key)
    return false;
  if (e.key == utils::Key::Enter || e.key == ' ') {
    open ? closePopup() : openPopup();
    return true;
  }
  if (e.key == utils::Key::Escape && open) {
    closePopup();
    return true;
  }
  return false;
}

void Dropdown::openPopup() {
  if (options.empty())
    return;
  open = true;

  // cap popup height to 10 items
  int popupH = std::min((int)options.size(), 10);

  core::Rect popupRect = {rect.x, rect.y + rect.height, rect.width, popupH};

  // clamp to terminal bottom
  core::Size term = core::Context::get().terminalSize;
  if (popupRect.y + popupRect.height > term.height) {
    // flip above if no room below
    popupRect.y = std::max(0, rect.y - popupH);
  }

  auto popup = std::make_shared<DropdownPopup>(
      options, selected,
      [this](int idx) {
        selected = idx;
        if (onChange)
          onChange(idx, options[idx].value);
        closePopup();
      },
      [this]() { closePopup(); },
      std::static_pointer_cast<Dropdown>(shared_from_this()));

  popup->setStyle(style);
  popup->setFocusedStyle(focusedStyle);

  core::Context::get().overlayManager.push(popup, popupRect);
}

void Dropdown::closePopup() {
  open = false;
  core::Context::get().overlayManager.pop();
}

DropdownPopup::DropdownPopup(const std::vector<Dropdown::Option> &options,
                             int initialSelected,
                             std::function<void(int)> onSelect,
                             std::function<void()> onClose,
                             std::weak_ptr<Dropdown> parentDropdown)
    : options(options), hovered(initialSelected),
      initialSelected(initialSelected), scrollOffset(0),
      parentDropdown(parentDropdown), onSelect(std::move(onSelect)),
      onClose(std::move(onClose)) {}

core::Size DropdownPopup::measure(int availW, int availH) {
  // cap popup height to 10 items
  int popupHeight = std::min((int)options.size(), 10);

  int width = availW;
  if (auto parent = parentDropdown.lock()) {
    width = parent->getWidth();
  }

  return {width, popupHeight};
}

void DropdownPopup::drawContent(backend::Buffer &buf, core::Rect inner) {
  const auto &s = activeStyle();

  // calculate how many items can fit and scroll range
  int visibleItems = inner.height;
  int maxScroll = std::max(0, (int)options.size() - visibleItems);
  scrollOffset = std::min(scrollOffset, maxScroll);

  // make sure hovered item stays visible
  if (hovered < scrollOffset)
    scrollOffset = hovered;
  if (hovered >= scrollOffset + visibleItems)
    scrollOffset = hovered - visibleItems + 1;

  // draw visible items
  for (int i = 0; i < visibleItems && (scrollOffset + i) < (int)options.size();
       ++i) {
    int optionIdx = scrollOffset + i;
    bool isHovered = (optionIdx == hovered);
    core::Color fg = isHovered ? s.bg : s.fg;
    core::Color bg = isHovered ? s.fg : s.bg;

    // fill row background
    for (int x = inner.x; x < inner.x + inner.width; ++x) {
      backend::Cell cell;
      cell.ch = " ";
      cell.fg = fg;
      cell.bg = bg;
      buf.set(x, inner.y + i, cell);
    }

    // determine if we need scroll indicators
    bool hasScrollAbove = (scrollOffset > 0 && i == 0);
    bool hasScrollBelow = (scrollOffset + visibleItems < (int)options.size() &&
                           i == visibleItems - 1);

    // draw label
    int x = inner.x;
    int maxX = inner.x + inner.width;

    // reserve space for right indicator
    if (hasScrollAbove || hasScrollBelow) {
      maxX--;
    }

    std::string displayLabel = options[optionIdx].label;
    size_t labelIdx = 0;
    while (labelIdx < displayLabel.size()) {
      size_t start = labelIdx;
      uint32_t cp = utils::unicode::nextCodepoint(displayLabel, labelIdx);
      if (labelIdx == start)
        break;

      int w = utils::unicode::displayWidth(cp);
      if (w <= 0)
        continue;
      if (x + w > maxX)
        break;

      backend::Cell cell;
      cell.ch = utils::unicode::encode(cp);
      cell.fg = fg;
      cell.bg = bg;
      buf.set(x, inner.y + i, cell);

      if (w == 2 && x + 1 < maxX) {
        backend::Cell wide;
        wide.ch = "";
        wide.fg = fg;
        wide.bg = bg;
        buf.set(x + 1, inner.y + i, wide);
      }

      x += w;
    }

    // draw right indicator
    if (hasScrollAbove) {
      backend::Cell cell;
      cell.ch = "↑";
      cell.fg = fg;
      cell.bg = bg;
      buf.set(inner.x + inner.width - 1, inner.y + i, cell);
    } else if (hasScrollBelow) {
      backend::Cell cell;
      cell.ch = "↓";
      cell.fg = fg;
      cell.bg = bg;
      buf.set(inner.x + inner.width - 1, inner.y + i, cell);
    }
  }
}

bool DropdownPopup::onEvent(const core::Event &e) {
  if (e.type != core::EventType::Key)
    return false;
  switch (e.key) {
  case utils::Key::Up:
    if (hovered > 0) {
      hovered--;
    }
    return true;
  case utils::Key::Down:
    if (hovered < (int)options.size() - 1) {
      hovered++;
    }
    return true;
  case utils::Key::Enter:
    if (onSelect)
      onSelect(hovered);
    return true;
  case utils::Key::Escape:
    // restore initial selection and close
    hovered = initialSelected;
    if (onClose)
      onClose();
    return true;
  default:
    return false;
  }
}

void Dropdown::layout(int x, int y, int width, int height) {
  Widget::layout(x, y, width, height);
}

} // namespace vextr::widgets