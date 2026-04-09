#include <Vextr/layout/AbsoluteLayout.hpp>
#include <Vextr/core/Widget.hpp>
#include <algorithm>

namespace vextr::core {

void AbsoluteLayout::apply(std::vector<ChildSlot>& children, Rect inner) {
    for (auto& slot : children) {
        int x = inner.x + slot.spec.absX.resolve(inner.width);
        int y = inner.y + slot.spec.absY.resolve(inner.height);

        int availW = std::max(0, inner.width  - slot.spec.absX.resolve(inner.width));
        int availH = std::max(0, inner.height - slot.spec.absY.resolve(inner.height));

        // resolve size
        int w, h;
        if (slot.spec.fixedW.isSet())
            w = slot.spec.fixedW.resolve(inner.width);
        else {
            Size s = slot.widget->measure(availW, availH);
            w = s.width;
        }
        if (slot.spec.fixedH.isSet())
            h = slot.spec.fixedH.resolve(inner.height);
        else {
            Size s = slot.widget->measure(availW, availH);
            h = s.height;
        }

        // clamp to min/max
        if (slot.spec.minW.isSet()) w = std::max(w, slot.spec.minW.resolve(inner.width));
        if (slot.spec.maxW.isSet()) w = std::min(w, slot.spec.maxW.resolve(inner.width));
        if (slot.spec.minH.isSet()) h = std::max(h, slot.spec.minH.resolve(inner.height));
        if (slot.spec.maxH.isSet()) h = std::min(h, slot.spec.maxH.resolve(inner.height));

        w = std::max(0, w);
        h = std::max(0, h);

        slot.widget->layout(x, y, w, h);
    }
}

} // vextr::core


