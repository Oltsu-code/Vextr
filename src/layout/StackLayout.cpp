#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <algorithm>

namespace vextr::layout {

void StackLayout::apply(std::vector<core::ChildSlot> &children,
                        core::Rect inner) {
  if (children.empty())
    return;

  bool isVert = (axis == Axis::Vertical);
  int mainSize = std::max(1, isVert ? inner.height : inner.width);
  int gapTotal = gap * (int)(children.size() - 1);
  int mainAvail = std::max(1, mainSize - gapTotal);

  // ── pass 1: resolve slot sizes ───────────────────────────────
  std::vector<int> sizes(children.size(), 0);
  float totalRatio = 0.f;
  int fixedUsed = 0;

  for (size_t i = 0; i < children.size(); ++i) {
    auto &slot = children[i];
    core::ScalarSpec f = isVert ? slot.spec.fixedH : slot.spec.fixedW;

    if (f.isSet()) {
      // fixed slot size — ignores ratio
      int s = f.resolve(mainSize);
      int mn = isVert ? slot.spec.minH.resolve(mainSize)
                      : slot.spec.minW.resolve(mainSize);
      int mx = isVert ? slot.spec.maxH.resolve(mainSize)
                      : slot.spec.maxW.resolve(mainSize);
      if (mx == 0)
        mx = mainSize;
      sizes[i] = std::max(1, std::clamp(s, mn, mx));
      fixedUsed += sizes[i];
    } else {
      totalRatio += slot.spec.ratio;
    }
  }

  // ── pass 2: distribute ratio pool ────────────────────────────
  int ratioPool = std::max(0, mainAvail - fixedUsed);

  for (size_t i = 0; i < children.size(); ++i) {
    auto &slot = children[i];
    core::ScalarSpec f = isVert ? slot.spec.fixedH : slot.spec.fixedW;
    if (f.isSet())
      continue;

    int s = totalRatio > 0.f
                ? static_cast<int>(ratioPool * slot.spec.ratio / totalRatio)
                : 0;

    int mn = isVert ? slot.spec.minH.resolve(mainAvail)
                    : slot.spec.minW.resolve(mainAvail);
    int mx = isVert ? slot.spec.maxH.resolve(mainAvail)
                    : slot.spec.maxW.resolve(mainAvail);
    if (mx == 0)
      mx = ratioPool;

    sizes[i] = std::max(1, std::clamp(s, mn, mx));
  }

  // fix rounding
  {
    int ratioUsed = 0;
    for (size_t i = 0; i < children.size(); ++i) {
      core::ScalarSpec f =
          isVert ? children[i].spec.fixedH : children[i].spec.fixedW;
      if (!f.isSet())
        ratioUsed += sizes[i];
    }
    int leftover = ratioPool - ratioUsed;
    if (leftover != 0) {
      for (int i = (int)children.size() - 1; i >= 0; --i) {
        core::ScalarSpec f =
            isVert ? children[i].spec.fixedH : children[i].spec.fixedW;
        if (!f.isSet()) {
          sizes[i] = std::max(1, sizes[i] + leftover);
          break;
        }
      }
    }
  }

  // ── pass 3: position each child ──────────────────────────────
  int cursor = isVert ? inner.y : inner.x;

  for (size_t i = 0; i < children.size(); ++i) {
    auto &slot = children[i];
    int slotSize = std::max(1, sizes[i]);
    int crossTotal = isVert ? inner.width : inner.height;

    // margins
    int mT = slot.spec.margin.resolvedTop(inner.height);
    int mB = slot.spec.margin.resolvedBottom(inner.height);
    int mL = slot.spec.margin.resolvedLeft(inner.width);
    int mR = slot.spec.margin.resolvedRight(inner.width);

    int crossMarA = isVert ? mL : mT;
    int crossMarB = isVert ? mR : mB;
    int mainMarA = isVert ? mT : mL;
    int mainMarB = isVert ? mB : mR;

    int crossSlot = std::max(1, crossTotal - crossMarA - crossMarB);
    int mainSlot = std::max(1, slotSize - mainMarA - mainMarB);

    // measure widget natural size
    core::Size desired = slot.widget->measure(isVert ? crossSlot : mainSlot,
                                              isVert ? mainSlot : crossSlot);

    // ── cross axis ───────────────────────────────────────────
    // sizeW (vertical) or sizeH (horizontal) = explicit widget size
    core::ScalarSpec crossSize = isVert ? slot.spec.sizeW : slot.spec.sizeH;
    core::Align crossAlign = isVert ? slot.spec.alignH : slot.spec.alignV;

    int contentCross;
    if (crossSize.isSet()) {
      contentCross = std::clamp(crossSize.resolve(crossSlot), 1, crossSlot);
    } else if (crossAlign == core::Align::Stretch) {
      contentCross = crossSlot;
    } else {
      int natural = std::max(1, isVert ? desired.width : desired.height);
      int mn = isVert ? slot.spec.minW.resolve(crossSlot)
                      : slot.spec.minH.resolve(crossSlot);
      int mx = isVert ? slot.spec.maxW.resolve(crossSlot)
                      : slot.spec.maxH.resolve(crossSlot);
      if (mx == 0)
        mx = crossSlot;
      contentCross = std::clamp(natural, mn, mx);
    }

    int crossOffset = 0;
    switch (crossAlign) {
    case core::Align::Center:
      crossOffset = (crossSlot - contentCross) / 2;
      break;
    case core::Align::End:
      crossOffset = crossSlot - contentCross;
      break;
    default:
      break;
    }
    int crossPos = (isVert ? inner.x : inner.y) + crossMarA + crossOffset;

    // main axis
    core::ScalarSpec mainSize2 = isVert ? slot.spec.sizeH : slot.spec.sizeW;
    core::Align mainAlign = isVert ? slot.spec.alignV : slot.spec.alignH;

    int contentMain;
    if (mainSize2.isSet()) {
      contentMain = std::clamp(mainSize2.resolve(mainSlot), 1, mainSlot);
    } else if (mainAlign == core::Align::Stretch) {
      contentMain = mainSlot;
    } else {
      int natural = std::max(1, isVert ? desired.height : desired.width);
      int mn = isVert ? slot.spec.minH.resolve(mainSlot)
                      : slot.spec.minW.resolve(mainSlot);
      int mx = isVert ? slot.spec.maxH.resolve(mainSlot)
                      : slot.spec.maxW.resolve(mainSlot);
      if (mx == 0)
        mx = mainSlot;
      contentMain = std::clamp(natural, mn, mx);
    }

    int mainOffset = 0;
    switch (mainAlign) {
    case core::Align::Center:
      mainOffset = (mainSlot - contentMain) / 2;
      break;
    case core::Align::End:
      mainOffset = mainSlot - contentMain;
      break;
    default:
      break;
    }
    int mainPos = cursor + mainMarA + mainOffset;

    core::Rect childRect;
    if (isVert)
      childRect = {crossPos, mainPos, contentCross, contentMain};
    else
      childRect = {mainPos, crossPos, contentMain, contentCross};

    slot.widget->layout(childRect.x, childRect.y, childRect.width,
                        childRect.height);
    cursor += slotSize + gap;
  }
}

} // namespace vextr::layout