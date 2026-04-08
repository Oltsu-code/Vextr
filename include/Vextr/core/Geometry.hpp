#pragma once

namespace vextr::core {

struct Size {
    int width = 0;
    int height = 0;
};

struct Pos {
    int x = 0;
    int y = 0;
};

struct Rect {
    int x = 0, y = 0;
    int width = 0, height = 0;

    int right()  const { return x + width; }
    int bottom() const { return y + height; }
};

} // vextr::core