#include <Vextr/layout/AbsoluteLayout.hpp>
#include <Vextr/core/Widget.hpp>

namespace vextr::core {

void AbsoluteLayout::apply(std::vector<ChildSlot>& children, Rect inner) {
    for (auto& slot : children) {
        int x = inner.x + slot.spec.absX;
        int y = inner.y + slot.spec.absY;
        Size s = slot.widget->measure(inner.width - slot.spec.absX,
                                      inner.height - slot.spec.absY);
        int w = slot.spec.fixedW > 0.f
            ? static_cast<int>(inner.width * slot.spec.fixedW / 100.f)
            : s.width;
        int h = slot.spec.fixedH > 0.f
            ? static_cast<int>(inner.height * slot.spec.fixedH / 100.f)
            : s.height;
        slot.widget->layout(x, y, w, h);
    }
}

} // vextr::core
