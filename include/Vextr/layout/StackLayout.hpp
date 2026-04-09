#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::core {

enum class Axis { Horizontal, Vertical };

class StackLayout : public Layout {
public:
    explicit StackLayout(Axis axis = Axis::Vertical, int gap = 0)
        : axis(axis), gap(gap) {}

    void apply(std::vector<ChildSlot>& children, Rect inner) override;

private:
    Axis axis;
    int  gap;

    Rect resolveSlot(const ChildSlot& slot, int x, int y, int w, int h);
};

} // vextr::core