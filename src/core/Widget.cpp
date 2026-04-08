#include <Vextr/core/Widget.hpp>

namespace vextr::core {

Size Widget::measure(int availW, int availH) {
    return { 0, 0 };
}

void Widget::layout(int x, int y, int width, int height) {
    rect = { x, y, width, height };
}

void Widget::render(backend::Buffer& buf) {
    backend::Cell cell;
    cell.ch = ' ';
    cell.bg = style.bg;
    cell.fg = style.fg;
    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            buf.set(x, y, cell);
}

} // vextr::core
