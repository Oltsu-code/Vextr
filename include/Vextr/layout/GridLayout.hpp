#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::core {

class GridLayout : public Layout {
public:
    GridLayout(int cols, int rows, int gapX = 0, int gapY = 0)
        : cols(cols), rows(rows), gapX(gapX), gapY(gapY) {}

    void apply(std::vector<ChildSlot>& children, Rect inner) override;

private:
    int cols, rows, gapX, gapY;
};

} // vextr::core
