#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/AbsoluteLayout.hpp>
#include <algorithm>

namespace vextr::layout {

void AbsoluteLayout::apply(std::vector<core::ChildSlot> &children,
                           core::Rect inner) {
  for (auto &slot : children) {
    int x = inner.x + slot.spec.absX.resolve(inner.width);
    int y = inner.y + slot.spec.absY.resolve(inner.height);

    int availW = std::max(0, inner.width - slot.spec.absX.resolve(inner.width));
    int availH =
        std::max(0, inner.height - slot.spec.absY.resolve(inner.height));

    // margins
    int mL = slot.spec.margin.resolvedLeft(availW);
    int mR = slot.spec.margin.resolvedRight(availW);
    int mT = slot.spec.margin.resolvedTop(availH);
    int mB = slot.spec.margin.resolvedBottom(availH);

    int innerX = x + mL;
    int innerY = y + mT;
    int innerW = std::max(0, availW - mL - mR);
    int innerH = std::max(0, availH - mT - mB);

    core::Size natural = slot.widget->measure(innerW, innerH);

    int w = slot.spec.sizeW.isSet()    ? slot.spec.sizeW.resolve(innerW)
            : slot.spec.fixedW.isSet() ? slot.spec.fixedW.resolve(innerW)
                                       : natural.width;
    int h = slot.spec.sizeH.isSet()    ? slot.spec.sizeH.resolve(innerH)
            : slot.spec.fixedH.isSet() ? slot.spec.fixedH.resolve(innerH)
                                       : natural.height;

    if (slot.spec.minW.isSet())
      w = std::max(w, slot.spec.minW.resolve(innerW));
    if (slot.spec.maxW.isSet())
      w = std::min(w, slot.spec.maxW.resolve(innerW));
    if (slot.spec.minH.isSet())
      h = std::max(h, slot.spec.minH.resolve(innerH));
    if (slot.spec.maxH.isSet())
      h = std::min(h, slot.spec.maxH.resolve(innerH));

    slot.widget->layout(innerX, innerY, std::max(0, w), std::max(0, h));
  }
}

} // namespace vextr::layout