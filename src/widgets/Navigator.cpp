#include <Vextr/core/Context.hpp>
#include <Vextr/widgets/Navigator.hpp>

namespace vextr::widgets {

void Navigator::push(std::shared_ptr<core::Widget> screen) {
  if (!screen)
    return;
  screen->parent = weak_from_this();
  stack.push_back(screen);
  screen->layout(rect.x, rect.y, rect.width, rect.height);
  core::Context::get().focusManager.clearFocus();
  if (onPush)
    onPush(screen);
}

void Navigator::pop() {
  if (stack.size() <= 1)
    return;
  auto popped = stack.back();
  stack.pop_back();
  core::Context::get().focusManager.clearFocus();
  if (onPop)
    onPop(popped);
}

void Navigator::replace(std::shared_ptr<core::Widget> screen) {
  if (!screen)
    return;
  auto old = stack.empty() ? nullptr : stack.back();
  if (!stack.empty())
    stack.pop_back();
  screen->parent = weak_from_this();
  stack.push_back(screen);
  screen->layout(rect.x, rect.y, rect.width, rect.height);
  core::Context::get().focusManager.clearFocus();
  if (onReplace)
    onReplace(old);
}

std::shared_ptr<core::Widget> Navigator::current() const {
  if (stack.empty())
    return nullptr;
  return stack.back();
}

core::Size Navigator::measure(int availW, int availH) {
  return {availW, availH};
}

void Navigator::layout(int x, int y, int width, int height) {
  core::Widget::layout(x, y, width, height);
  // layout all screens to the same rect
  // bottom screens need to know their size for rendering
  for (auto &screen : stack)
    screen->layout(x, y, width, height);
}

void Navigator::render(backend::Buffer &buf) {
  // render bottom to top so overlays paint over lower screens
  for (auto &screen : stack)
    screen->render(buf);
}

bool Navigator::onEvent(const core::Event &e) {
  // only top screen receives events
  if (auto top = current())
    return top->onEvent(e);
  return false;
}

} // namespace vextr::widgets