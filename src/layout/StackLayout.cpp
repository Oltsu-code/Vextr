#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/core/Widget.hpp>
#include <algorithm>

namespace vextr::core {

void StackLayout::apply(std::vector<ChildSlot>& children, Rect inner) {
    if (children.empty()) return;

    bool isVert   = (axis == Axis::Vertical);
    int  mainSize = isVert ? inner.height : inner.width;
    int  gapTotal = gap * (int)(children.size() - 1);
    int  mainAvail = std::max(0, mainSize - gapTotal);

    // pass 1: resolve fixed sizes, sum up ratio pool
    std::vector<int> sizes(children.size(), 0);
    float totalRatio = 0.f;
    int   fixedUsed  = 0;

    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot = children[i];
        ScalarSpec f = isVert ? slot.spec.fixedH : slot.spec.fixedW;
        if (f.isSet()) {
            int s = f.resolve(mainAvail);
            // clamp to min/max
            int mn = isVert ? slot.spec.minH.resolve(mainAvail)
                            : slot.spec.minW.resolve(mainAvail);
            int mx = isVert ? slot.spec.maxH.resolve(mainAvail)
                            : slot.spec.maxW.resolve(mainAvail);
            if (mx == 0) mx = mainAvail;
            s = std::clamp(s, mn, mx);
            sizes[i]  = s;
            fixedUsed += s;
        } else {
            totalRatio += slot.spec.ratio;
        }
    }

    // pass 2: distribute ratio pool
    int ratioPool = std::max(0, mainAvail - fixedUsed);
    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot = children[i];
        ScalarSpec f = isVert ? slot.spec.fixedH : slot.spec.fixedW;
        if (!f.isSet()) {
            int s = totalRatio > 0.f
                ? static_cast<int>(ratioPool * slot.spec.ratio / totalRatio)
                : 0;
            int mn = isVert ? slot.spec.minH.resolve(mainAvail)
                            : slot.spec.minW.resolve(mainAvail);
            int mx = isVert ? slot.spec.maxH.resolve(mainAvail)
                            : slot.spec.maxW.resolve(mainAvail);
            if (mx == 0) mx = ratioPool;
            sizes[i] = std::clamp(s, mn, mx);
        }
    }

    // fix rounding
    {
        int used = 0;
        for (int s : sizes) used += s;
        int leftover = mainAvail - fixedUsed - used + fixedUsed;
        int ratioUsed = 0;
        for (size_t i = 0; i < children.size(); ++i) {
            ScalarSpec f = isVert ? children[i].spec.fixedH : children[i].spec.fixedW;
            if (!f.isSet()) ratioUsed += sizes[i];
        }
        int leftover2 = ratioPool - ratioUsed;
        if (leftover2 != 0) {
            for (int i = (int)children.size() - 1; i >= 0; --i) {
                ScalarSpec f = isVert ? children[i].spec.fixedH : children[i].spec.fixedW;
                if (!f.isSet()) { sizes[i] += leftover2; break; }
            }
        }
    }

    // pass 3: position each child
    int cursor = isVert ? inner.y : inner.x;

    for (size_t i = 0; i < children.size(); ++i) {
        auto& slot  = children[i];
        int   mSz   = sizes[i];
        int   crossTotal = isVert ? inner.width : inner.height;

        // resolve margins
        int mT = slot.spec.margin.resolvedTop   (inner.height);
        int mB = slot.spec.margin.resolvedBottom(inner.height);
        int mL = slot.spec.margin.resolvedLeft  (inner.width);
        int mR = slot.spec.margin.resolvedRight (inner.width);

        int crossMarA = isVert ? mL : mT;
        int crossMarB = isVert ? mR : mB;
        int mainMarA  = isVert ? mT : mL;
        int mainMarB  = isVert ? mB : mR;

        int crossInner = std::max(0, crossTotal - crossMarA - crossMarB);
        int mainInner  = std::max(0, mSz - mainMarA - mainMarB);

        // cross axis
        Align crossAlign = isVert ? slot.spec.alignH : slot.spec.alignV;
        int contentCross;
        if (crossAlign == Align::Stretch) {
            contentCross = crossInner;
        } else {
            Size desired = slot.widget->measure(
                isVert ? crossInner : mainInner,
                isVert ? mainInner  : crossInner
            );
            int natural = isVert ? desired.width : desired.height;
            contentCross = std::clamp(natural, 0, crossInner);
        }

        int crossOffset = 0;
        switch (crossAlign) {
            case Align::Center: crossOffset = (crossInner - contentCross) / 2; break;
            case Align::End:    crossOffset = crossInner - contentCross;        break;
            default: break;
        }
        int crossPos = (isVert ? inner.x : inner.y) + crossMarA + crossOffset;

        // main axis
        Align mainAlign = isVert ? slot.spec.alignV : slot.spec.alignH;
        int contentMain;
        if (mainAlign == Align::Stretch) {
            contentMain = mainInner;
        } else {
            Size desired = slot.widget->measure(
                isVert ? contentCross : mainInner,
                isVert ? mainInner   : contentCross
            );
            int natural = isVert ? desired.height : desired.width;
            contentMain = std::clamp(natural, 0, mainInner);
        }

        int mainOffset = 0;
        switch (mainAlign) {
            case Align::Center: mainOffset = (mainInner - contentMain) / 2; break;
            case Align::End:    mainOffset = mainInner - contentMain;        break;
            default: break;
        }
        int mainPos = cursor + mainMarA + mainOffset;

        Rect childRect;
        if (isVert)
            childRect = { crossPos, mainPos, contentCross, contentMain };
        else
            childRect = { mainPos, crossPos, contentMain, contentCross };

        slot.widget->layout(childRect.x, childRect.y, childRect.width, childRect.height);
        cursor += mSz + gap;
    }
}

} // vextr::core