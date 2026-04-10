#include <Vextr/core/Widget.hpp>
#include <Vextr/core/Context.hpp>

namespace vextr::core {

bool Widget::focused() const {
    return Context::get().focusManager.focused().get() == this;
}

const Style& Widget::activeStyle() const {
    return focused() ? focusedStyle : style;
}

Size Widget::measure(int availW, int availH) {
    return { 0, 0 };
}

void Widget::layout(int x, int y, int width, int height) {
    rect = { x, y, width, height };
}

void Widget::render(backend::Buffer& buf) {
    const Style& s = activeStyle();
    backend::Cell cell;
    cell.ch = ' ';
    cell.bg = s.bg;
    cell.fg = s.fg;
    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            buf.set(x, y, cell);
}

} // vextr::core
