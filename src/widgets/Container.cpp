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
  layoutEngine->apply(children, innerRect());
}

void Container::drawContent(backend::Buffer &buf, Rect inner) {
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