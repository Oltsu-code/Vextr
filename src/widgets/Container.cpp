#include <Vextr/widgets/Container.hpp>

namespace vextr::core {

void Container::addChild(std::shared_ptr<Widget> child, LayoutSpec spec) {
  child->parent = shared_from_this();
  children.push_back({child, spec});
}

Size Container::measure(int availW, int availH) { return {availW, availH}; }

void Container::layout(int x, int y, int width, int height) {
  Widget::layout(x, y, width, height);
  if (children.empty())
    return;

  int p = style.padding;
  Rect inner = {x + p, y + p, std::max(0, width - p * 2),
                std::max(0, height - p * 2)};

  layoutEngine->apply(children, inner);
}

void Container::render(backend::Buffer &buf) {
  Widget::render(buf);
  for (auto &slot : children)
    slot.widget->render(buf);
}

bool Container::onEvent(const Event &e) {
  for (auto &slot : children)
    if (slot.widget->onEvent(e))
      return true;
  return false;
}

} // namespace vextr::core
