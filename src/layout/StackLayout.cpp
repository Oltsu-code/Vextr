#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/core/Align.hpp>
#include <algorithm>
#include <climits>

namespace vextr::core {

static int applyMarginAndAlign(int pos, int size, int contentSize,
                                int marginA, int marginB, Align align) {
    int available = size - marginA - marginB;
    int start     = pos + marginA;

    switch (align) {
        case Align::Center:  return start + (available - contentSize) / 2;
        case Align::End:     return start + available - contentSize;
        case Align::Stretch: return start; // contentSize == available
        default:             return start;
    }
}

void StackLayout::apply(std::vector<ChildSlot>& children, Rect inner) {
    if (children.empty()) return;

    bool isVert = (axis == Axis::Vertical);

    // first pass: measure fixed/percent children, collect expand ratio total
    float totalRatio   = 0.f;
    int   fixedUsed    = 0;
    int   gapUsed      = gap * std::max(0, (int)children.size() - 1);
    int   mainAvail    = (isVert ? inner.height : inner.width) - gapUsed;

    std::vector<int> sizes(children.size(), 0);

    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot = children[i];
        float fw = slot.spec.fixedW;
        float fh = slot.spec.fixedH;
        float f  = isVert ? fh : fw;

        if (f > 0.f) {
            int s = static_cast<int>((isVert ? inner.height : inner.width) * f / 100.f);
            s = std::clamp(s, slot.spec.minW, slot.spec.maxW);
            sizes[i] = s;
            fixedUsed += s;
        } else {
            totalRatio += slot.spec.ratio;
        }
    }

    int remaining = std::max(0, mainAvail - fixedUsed);

    // second pass: assign ratio-based sizes
    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot = children[i];
        float fw = slot.spec.fixedW;
        float fh = slot.spec.fixedH;
        float f  = isVert ? fh : fw;
        if (f <= 0.f) {
            int s = (totalRatio > 0.f)
                ? static_cast<int>(remaining * slot.spec.ratio / totalRatio)
                : 0;
            s = std::clamp(s, slot.spec.minH, slot.spec.maxH);
            sizes[i] = s;
        }
    }

    // give leftover pixels to last child to avoid rounding gaps
    int total = gapUsed + fixedUsed;
    for (int s : sizes) total += s;
    if (!sizes.empty()) sizes.back() += std::max(0, mainAvail - (total - gapUsed - fixedUsed));

    // third pass: layout each child
    int cursor = isVert ? inner.y : inner.x;

    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot   = children[i];
        int   mL     = slot.spec.marginLeft;
        int   mR     = slot.spec.marginRight;
        int   mT     = slot.spec.marginTop;
        int   mB     = slot.spec.marginBottom;
        int   mainSz = sizes[i];

        // cross axis size
        int crossAvail = (isVert ? inner.width : inner.height);
        Align crossAlign = isVert ? slot.spec.alignH : slot.spec.alignV;
        int   crossMarA  = isVert ? mL : mT;
        int   crossMarB  = isVert ? mR : mB;

        // measure for cross axis if not stretching
        int contentCross = crossAvail - crossMarA - crossMarB;
        if (crossAlign != Align::Stretch) {
            Size desired = slot.widget->measure(
                isVert ? contentCross : mainSz,
                isVert ? mainSz : contentCross
            );
            contentCross = std::clamp(
                isVert ? desired.width : desired.height,
                0, crossAvail - crossMarA - crossMarB
            );
        }

        // resolve cross position
        int crossPos = applyMarginAndAlign(
            isVert ? inner.x : inner.y,
            isVert ? inner.width : inner.height,
            contentCross, crossMarA, crossMarB, crossAlign
        );

        // resolve main position (margin along main axis)
        int mainStart = cursor + (isVert ? mT : mL);
        int mainSize  = mainSz - (isVert ? mT + mB : mL + mR);
        mainSize      = std::max(0, mainSize);

        Rect childRect;
        if (isVert) {
            childRect = { crossPos, mainStart, contentCross, mainSize };
        } else {
            childRect = { mainStart, crossPos, mainSize, contentCross };
        }

        slot.widget->layout(childRect.x, childRect.y, childRect.width, childRect.height);
        cursor += mainSz + gap;
    }
}

} // vextr::core
