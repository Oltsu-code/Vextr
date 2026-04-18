#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/ScrollView.hpp>
#include <algorithm>
#include <utility>

namespace {

vextr::backend::Cell transparentCell() {
  vextr::backend::Cell cell;
  cell.ch = "";
  cell.bg = vextr::core::Color::clear();
  return cell;
}

} // namespace

namespace vextr::widgets {

ScrollView::ScrollView(std::shared_ptr<core::Widget> child) {
  setContent(std::move(child));
}

void ScrollView::setContent(std::shared_ptr<core::Widget> child) {
  content = std::move(child);
  if (content)
    content->parent = weak_from_this();
  if (hasLayout)
    relayoutContent();
}

void ScrollView::setContentSize(core::Size size) {
  explicitContentSize = size;
  hasExplicitContentSize = true;
  if (hasLayout)
    relayoutContent();
}

void ScrollView::clearContentSize() {
  hasExplicitContentSize = false;
  if (hasLayout)
    relayoutContent();
}

void ScrollView::setScroll(int x, int y) {
  scroll = {x, y};
  if (hasLayout)
    relayoutContent();
}

void ScrollView::scrollBy(int dx, int dy) {
  setScroll(scroll.x + dx, scroll.y + dy);
}

core::Size ScrollView::measure(int availW, int availH) {
  return {availW, availH};
}

core::Size ScrollView::resolveContentSize(core::Rect inner) const {
  if (hasExplicitContentSize)
    return {explicitContentSize.width > 0 ? explicitContentSize.width
                                          : std::max(0, inner.width),
            explicitContentSize.height > 0 ? explicitContentSize.height
                                           : std::max(0, inner.height)};
  if (!content)
    return {0, 0};
  return content->measure(std::max(0, inner.width), std::max(0, inner.height));
}

void ScrollView::clampScroll() {
  int maxX = std::max(0, currentContentSize.width - currentInner.width);
  int maxY = std::max(0, currentContentSize.height - currentInner.height);
  scroll.x = std::clamp(scroll.x, 0, maxX);
  scroll.y = std::clamp(scroll.y, 0, maxY);
}

void ScrollView::relayoutContent() {
  if (!hasLayout)
    return;

  if (content && content->parent.expired())
    content->parent = weak_from_this();

  currentContentSize = resolveContentSize(currentInner);
  clampScroll();

  if (content)
    content->layout(-scroll.x, -scroll.y, currentContentSize.width,
                    currentContentSize.height);
}

void ScrollView::layout(int x, int y, int width, int height) {
  core::Widget::layout(x, y, width, height);
  hasLayout = true;
  currentInner = innerRect();
  relayoutContent();
}

bool ScrollView::onEvent(const core::Event &event) {
  if (event.type != core::EventType::Key)
    return false;

  switch (event.key) {
  case utils::Key::Up:
    scrollBy(0, -1);
    return true;
  case utils::Key::Down:
    scrollBy(0, 1);
    return true;
  case utils::Key::Left:
    scrollBy(-1, 0);
    return true;
  case utils::Key::Right:
    scrollBy(1, 0);
    return true;
  case utils::Key::PageUp:
    scrollBy(0, -std::max(1, currentInner.height));
    return true;
  case utils::Key::PageDown:
    scrollBy(0, std::max(1, currentInner.height));
    return true;
  case utils::Key::Home:
    setScroll(0, 0);
    return true;
  case utils::Key::End:
    setScroll(currentContentSize.width, currentContentSize.height);
    return true;
  default:
    return false;
  }
}

void ScrollView::drawContent(backend::Buffer &buf, core::Rect inner) {
  if (!content || inner.width <= 0 || inner.height <= 0)

    return;

  core::Size visibleSize = {inner.width, inner.height};
  backend::Buffer viewport(visibleSize.width, visibleSize.height);
  for (int yy = 0; yy < viewport.height(); ++yy)

    for (int xx = 0; xx < viewport.width(); ++xx)
      viewport.set(xx, yy, transparentCell());

  content->render(viewport);

  for (int yy = 0; yy < viewport.height(); ++yy) {

    for (int xx = 0; xx < viewport.width(); ++xx) {
      const auto &cell = viewport.get(xx, yy);
      if (cell.ch.empty() && cell.bg.transparent && !cell.bold &&
          !cell.underline)
        continue;
      buf.set(inner.x + xx, inner.y + yy, cell);
    }
  }
}

} // namespace vextr::widgets
