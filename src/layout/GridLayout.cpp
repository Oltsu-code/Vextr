#include <Vextr/layout/GridLayout.hpp>
#include <Vextr/core/Widget.hpp>
#include <algorithm>

namespace vextr::core {

void GridLayout::apply(std::vector<ChildSlot>& children, Rect inner) {
    if (cols <= 0 || rows <= 0) return;

    int cellW = (inner.width  - gapX * (cols - 1)) / cols;
    int cellH = (inner.height - gapY * (rows - 1)) / rows;

    for (auto& slot : children) {
        int c = std::clamp(slot.spec.col, 0, cols - 1);
        int r = std::clamp(slot.spec.row, 0, rows - 1);
        int cSpan = std::clamp(slot.spec.colSpan, 1, cols - c);
        int rSpan = std::clamp(slot.spec.rowSpan, 1, rows - r);

        int x = inner.x + c * (cellW + gapX);
        int y = inner.y + r * (cellH + gapY);
        int w = cellW * cSpan + gapX * (cSpan - 1);
        int h = cellH * rSpan + gapY * (rSpan - 1);

        w = std::clamp(w, slot.spec.minW, slot.spec.maxW);
        h = std::clamp(h, slot.spec.minH, slot.spec.maxH);

        slot.widget->layout(x, y, w, h);
    }
}

} // vextr::core
