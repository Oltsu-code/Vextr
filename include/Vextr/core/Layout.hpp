#pragma once
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/Align.hpp>
#include <memory>
#include <vector>
#include <climits>

namespace vextr::core {

class Widget;

struct LayoutSpec {
    float ratio   = 1.0f;
    float fixedW  = 0.f;   // % of parent width,  0 = ignore
    float fixedH  = 0.f;   // % of parent height, 0 = ignore
    int   minW    = 0;
    int   minH    = 0;
    int   maxW    = INT_MAX;
    int   maxH    = INT_MAX;
    Align alignH  = Align::Stretch;
    Align alignV  = Align::Stretch;
    int   marginTop    = 0;
    int   marginBottom = 0;
    int   marginLeft   = 0;
    int   marginRight  = 0;
    // grid only
    int   col     = 0;
    int   row     = 0;
    int   colSpan = 1;
    int   rowSpan = 1;
    // absolute only
    int   absX    = 0;
    int   absY    = 0;
};

struct ChildSlot {
    std::shared_ptr<Widget> widget;
    LayoutSpec spec;
};

class Layout {
public:
    virtual ~Layout() = default;
    virtual void apply(std::vector<ChildSlot>& children, Rect inner) = 0;
};

} // vextr::core
