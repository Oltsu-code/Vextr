#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/AbsoluteLayout.hpp>
#include <algorithm>

namespace vextr::layout {

void AbsoluteLayout::apply(std::vector<core::ChildSlot> &children, core::Rect inner) {
  for (auto &slot : children) {
    int x = inner.x + slot.spec.absX.resolve(inner.width);
    int y = inner.y + slot.spec.absY.resolve(inner.height);

    int availW = std::max(0, inner.width - slot.spec.absX.resolve(inner.width));
    int availH =
        std::max(0, inner.height - slot.spec.absY.resolve(inner.height));

    core::Size natural = slot.widget->measure(availW, availH);

    // sizeW/sizeH = explicit widget size
    // fixedW/fixedH = fallback if sizeW/sizeH not set
    int w = slot.spec.sizeW.isSet()    ? slot.spec.sizeW.resolve(inner.width)
            : slot.spec.fixedW.isSet() ? slot.spec.fixedW.resolve(inner.width)
                                       : natural.width;

    int h = slot.spec.sizeH.isSet()    ? slot.spec.sizeH.resolve(inner.height)
            : slot.spec.fixedH.isSet() ? slot.spec.fixedH.resolve(inner.height)
                                       : natural.height;

    if (slot.spec.minW.isSet())
      w = std::max(w, slot.spec.minW.resolve(inner.width));
    if (slot.spec.maxW.isSet())
      w = std::min(w, slot.spec.maxW.resolve(inner.width));
    if (slot.spec.minH.isSet())
      h = std::max(h, slot.spec.minH.resolve(inner.height));
    if (slot.spec.maxH.isSet())
      h = std::min(h, slot.spec.maxH.resolve(inner.height));

    slot.widget->layout(x, y, std::max(0, w), std::max(0, h));
  }
}

} // namespace vextr::core