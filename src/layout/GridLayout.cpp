#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/GridLayout.hpp>
#include <algorithm>

namespace vextr::layout {

void GridLayout::apply(std::vector<core::ChildSlot> &children, core::Rect inner) {
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

    // margins
    int mT = slot.spec.margin.resolvedTop(slotH);
    int mB = slot.spec.margin.resolvedBottom(slotH);
    int mL = slot.spec.margin.resolvedLeft(slotW);
    int mR = slot.spec.margin.resolvedRight(slotW);

    int innerX = slotX + mL;
    int innerY = slotY + mT;
    int innerW = std::max(0, slotW - mL - mR);
    int innerH = std::max(0, slotH - mT - mB);

    core::Size natural = slot.widget->measure(innerW, innerH);

    // widget size within cell
    int finalW = slot.spec.sizeW.isSet() ? slot.spec.sizeW.resolve(innerW)
                 : slot.spec.alignH == core::Align::Stretch
                     ? innerW
                     : std::clamp(natural.width, 0, innerW);

    int finalH = slot.spec.sizeH.isSet() ? slot.spec.sizeH.resolve(innerH)
                 : slot.spec.alignV == core::Align::Stretch
                     ? innerH
                     : std::clamp(natural.height, 0, innerH);

    // clamp to min/max
    if (slot.spec.minW.isSet())
      finalW = std::max(finalW, slot.spec.minW.resolve(innerW));
    if (slot.spec.maxW.isSet())
      finalW = std::min(finalW, slot.spec.maxW.resolve(innerW));
    if (slot.spec.minH.isSet())
      finalH = std::max(finalH, slot.spec.minH.resolve(innerH));
    if (slot.spec.maxH.isSet())
      finalH = std::min(finalH, slot.spec.maxH.resolve(innerH));

    // alignment within cell
    int offX = 0, offY = 0;
    switch (slot.spec.alignH) {
    case core::Align::Center:
      offX = (innerW - finalW) / 2;
      break;
    case core::Align::End:
      offX = innerW - finalW;
      break;
    default:
      break;
    }
    switch (slot.spec.alignV) {
    case core::Align::Center:
      offY = (innerH - finalH) / 2;
      break;
    case core::Align::End:
      offY = innerH - finalH;
      break;
    default:
      break;
    }

    slot.widget->layout(innerX + offX, innerY + offY, std::max(0, finalW),
                        std::max(0, finalH));
  }
}

} // namespace vextr::core