#include <Vextr/core/Context.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/utils/Input.hpp>

namespace vextr::core {

void FocusManager::setFocus(std::shared_ptr<Widget> w) {
  auto prev = current.lock();
  if (prev == w)
    return;
  if (prev)
    prev->onBlur();
  current = w;
  if (w)
    w->onFocus();
}

void FocusManager::clearFocus() {
  if (auto w = current.lock())
    w->onBlur();
  current.reset();
}

void FocusManager::collectFocusable(std::shared_ptr<Widget> w,
                                    std::vector<std::shared_ptr<Widget>> &out) {
  if (!w)
    return;
  if (w->isFocusable())
    out.push_back(w);
  for (auto &child : w->getChildren())
    collectFocusable(child, out);
}

void FocusManager::focusNext(std::shared_ptr<Widget> root) {
  std::vector<std::shared_ptr<Widget>> focusable;
  collectFocusable(root, focusable);
  if (focusable.empty())
    return;

  auto cur = current.lock();
  if (!cur) {
    setFocus(focusable.front());
    return;
  }

  for (size_t i = 0; i < focusable.size(); ++i) {
    if (focusable[i] == cur) {
      setFocus(focusable[(i + 1) % focusable.size()]);
      return;
    }
  }
  setFocus(focusable.front());
}

void FocusManager::focusPrev(std::shared_ptr<Widget> root) {
  std::vector<std::shared_ptr<Widget>> focusable;
  collectFocusable(root, focusable);
  if (focusable.empty())
    return;

  auto cur = current.lock();
  if (!cur) {
    setFocus(focusable.back());
    return;
  }

  for (size_t i = 0; i < focusable.size(); ++i) {
    if (focusable[i] == cur) {
      setFocus(focusable[(i == 0 ? focusable.size() : i) - 1]);
      return;
    }
  }
  setFocus(focusable.back());
}

bool FocusManager::dispatch(const Event &e, std::shared_ptr<Widget> root) {
  if (e.type == EventType::Key) {
    if (e.key == utils::Key::Tab) {
      focusNext(root);
      return true;
    }
    if (e.key == utils::Key::ShiftTab) {
      focusPrev(root);
      return true;
    }
    if (e.key == utils::Key::Escape) {
      clearFocus();
      return true;
    }
  }

  // route to focused widget
  auto fw = focused();
  if (!fw)
    return false;

  if (fw->onEvent(e))
    return true;

  // bubble up
  auto p = fw->parent.lock();
  while (p) {
    if (p->onEvent(e))
      return true;
    p = p->parent.lock();
  }
  return false;
}

} // namespace vextr::core