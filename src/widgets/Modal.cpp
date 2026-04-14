#include <Vextr/core/Context.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/Modal.hpp>
#include <algorithm>

namespace vextr::widgets {

Modal::Modal(std::shared_ptr<Widget> content, std::function<void()> onClose,
             bool dismissable)
    : content(std::move(content)), onClose(std::move(onClose)),
      dismissable(dismissable) {
  style.bg = core::Color::clear();
}

core::Size Modal::measure(int availW, int availH) { return {availW, availH}; }

void Modal::layout(int x, int y, int width, int height) {
  Widget::layout(x, y, width, height);

  // center content box
  int cw = std::max(10, (int)(width * contentW));
  int ch = std::max(5, (int)(height * contentH));
  int cx = x + (width - cw) / 2;
  int cy = y + (height - ch) / 2;
  content->layout(cx, cy, cw, ch);
}

void Modal::handleResize(int termWidth, int termHeight) {
  layout(0, 0, termWidth, termHeight);
}

void Modal::drawContent(backend::Buffer &buf, core::Rect inner) {
  for (int y = inner.y; y < inner.y + inner.height; ++y) {
    for (int x = inner.x; x < inner.x + inner.width; ++x) {
      if (x < 0 || y < 0 || x >= buf.width() || y >= buf.height())
        continue;
      backend::Cell cell = buf.get(x, y);
      cell.fg.r /= 2;
      cell.fg.g /= 2;
      cell.fg.b /= 2;
      cell.bg.r /= 2;
      cell.bg.g /= 2;
      cell.bg.b /= 2;
      buf.set(x, y, cell);
    }
  }
}

bool Modal::onEvent(const core::Event &e) {
  // forward to content first
  if (content->onEvent(e))
    return true;

  if (dismissable && e.type == core::EventType::Key &&
      e.key == utils::Key::Escape) {
    if (onClose)
      onClose();
    core::Context::get().overlayManager.pop();
    return true;
  }
  return false;
}

void showModal(std::shared_ptr<core::Widget> content,
               std::function<void()> onClose, bool dismissable) {
  auto &ctx = core::Context::get();
  auto modal = std::make_shared<Modal>(std::move(content), std::move(onClose),
                                       dismissable);
  core::Rect full = {0, 0, ctx.terminalSize.width, ctx.terminalSize.height};
  ctx.overlayManager.push(modal, full);
}

void closeModal() { core::Context::get().overlayManager.pop(); }

} // namespace vextr::widgets