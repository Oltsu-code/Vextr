#include <Vextr/widgets/Container.hpp>

namespace vextr::widgets {

void Container::addChild(std::shared_ptr<core::Widget> child,
                         core::LayoutSpec spec) {
  child->parent = shared_from_this();
  children.push_back({child, spec});
}

core::Size Container::measure(int availW, int availH) {
  return {availW, availH};
}

void Container::layout(int x, int y, int width, int height) {
  Widget::layout(x, y, width, height);
  if (children.empty())
    return;
  layoutEngine->apply(children, innerRect());
}

void Container::drawContent(backend::Buffer &buf, core::Rect inner) {
  for (auto &slot : children)
    slot.widget->render(buf);
}

} // namespace vextr::widgets