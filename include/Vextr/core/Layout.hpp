#pragma once
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/Align.hpp>
#include <memory>
#include <vector>
#include <climits>

namespace vextr::core {

class Widget;

struct ScalarSpec {
    float value  = 0.f;
    bool percent = false;

    bool isSet() const { return value > 0.f; }

    int resolve(int parentSize) const {
        if (!isSet()) return 0;
        if (percent) return static_cast<int>(parentSize * value / 100.f);
        return static_cast<int>(value);
    }
};

inline ScalarSpec pct(float v)   { return {v, true};  }
inline ScalarSpec cells(float v) { return {v, false}; }

struct MarginSpec {
    ScalarSpec top, bottom, left, right;

    static MarginSpec all(ScalarSpec v)             { return {v, v, v, v}; }
    static MarginSpec xy(ScalarSpec x, ScalarSpec y){ return {y, y, x, x}; }
    static MarginSpec none()                        { return {}; }

    int resolvedTop   (int h) const { return top.resolve(h);    }
    int resolvedBottom(int h) const { return bottom.resolve(h); }
    int resolvedLeft  (int w) const { return left.resolve(w);   }
    int resolvedRight (int w) const { return right.resolve(w);  }
};

struct LayoutSpec {
    float      ratio  = 1.0f;
    ScalarSpec fixedW;
    ScalarSpec fixedH;
    ScalarSpec minW;
    ScalarSpec minH;
    ScalarSpec maxW;
    ScalarSpec maxH;
    Align      alignH = Align::Stretch;
    Align      alignV = Align::Stretch;
    MarginSpec margin;
    // grid
    int col     = 0;
    int row     = 0;
    int colSpan = 1;
    int rowSpan = 1;
    // absolute
    ScalarSpec absX;
    ScalarSpec absY;
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
