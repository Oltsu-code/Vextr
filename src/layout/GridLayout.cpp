#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/GridLayout.hpp>
#include <algorithm>

namespace vextr::core {

void GridLayout::apply(std::vector<ChildSlot> &children, Rect inner) {
  if (cols <= 0 || rows <= 0 || children.empty())
    return;

  int cellW = std::max(1, (inner.width - gapX * (cols - 1)) / cols);
  int cellH = std::max(1, (inner.height - gapY * (rows - 1)) / rows);

  for (auto &slot : children) {
    int c = std::clamp(slot.spec.col, 0, cols - 1);
    int r = std::clamp(slot.spec.row, 0, rows - 1);
    int cSpan = std::clamp(slot.spec.colSpan, 1, cols - c);
    int rSpan = std::clamp(slot.spec.rowSpan, 1, rows - r);

    int slotX = inner.x + c * (cellW + gapX);
    int slotY = inner.y + r * (cellH + gapY);
    int slotW = cellW * cSpan + gapX * (cSpan - 1);
    int slotH = cellH * rSpan + gapY * (rSpan - 1);

    // resolve margins
    int mT = slot.spec.margin.resolvedTop(slotH);
    int mB = slot.spec.margin.resolvedBottom(slotH);
    int mL = slot.spec.margin.resolvedLeft(slotW);
    int mR = slot.spec.margin.resolvedRight(slotW);

    int innerX = slotX + mL;
    int innerY = slotY + mT;
    int innerW = std::max(0, slotW - mL - mR);
    int innerH = std::max(0, slotH - mT - mB);

    // alignment
    int finalW = innerW, finalH = innerH;
    int offX = 0, offY = 0;

    bool needMeasure = slot.spec.alignH != Align::Stretch ||
                       slot.spec.alignV != Align::Stretch;
    if (needMeasure) {
      Size desired = slot.widget->measure(innerW, innerH);

      if (slot.spec.alignH != Align::Stretch) {
        finalW = std::clamp(desired.width, 0, innerW);
        switch (slot.spec.alignH) {
        case Align::Center:
          offX = (innerW - finalW) / 2;
          break;
        case Align::End:
          offX = innerW - finalW;
          break;
        default:
          break;
        }
      }
      if (slot.spec.alignV != Align::Stretch) {
        finalH = std::clamp(desired.height, 0, innerH);
        switch (slot.spec.alignV) {
        case Align::Center:
          offY = (innerH - finalH) / 2;
          break;
        case Align::End:
          offY = innerH - finalH;
          break;
        default:
          break;
        }
      }
    }

    slot.widget->layout(innerX + offX, innerY + offY, finalW, finalH);
  }
}

} // namespace vextr::core
