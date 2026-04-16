#include <Vextr/core/Context.hpp>
#include <Vextr/core/OverlayManager.hpp>
#include <Vextr/core/Widget.hpp>

namespace vextr::core {

void OverlayManager::push(std::shared_ptr<Widget> widget, Rect rect) {
  widget->layout(rect.x, rect.y, rect.width, rect.height);
  overlays.emplace_back(std::move(widget), rect);
  // focus first focusable in the new overlay
  Context::get().focusManager.focusTopOverlay();
}

void OverlayManager::pop() {
  if (overlays.empty())
    return;
  overlays.pop_back();
  // refocus main tree or next overlay down
  auto &fm = Context::get().focusManager;
  if (!overlays.empty())
    fm.focusTopOverlay();
  else
    fm.clearFocus(); // TODO: make this return where focus was. maybe trough
                     // saving the last focused thing
}

void OverlayManager::clear() { overlays.clear(); }

void OverlayManager::resize(int termWidth, int termHeight) {
  for (auto &[widget, rect] : overlays) {
    Size measured = widget->measure(termWidth, termHeight);

    rect.width = measured.width;
    rect.height = measured.height;

    widget->layout(rect.x, rect.y, rect.width, rect.height);
  }
}

} // namespace vextr::core
