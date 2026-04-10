#include <Vextr/widgets/Button.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/utils/Unicode.hpp>

namespace vextr::widgets {

core::Size Button::measure(int availW, int availH) {
    return { utils::unicode::stringWidth(label) + 4, 1 };
}

void Button::render(backend::Buffer& buf) {
    const core::Style& s = activeStyle();

    // fill background
    backend::Cell base;
    base.ch = ' ';
    base.fg = s.fg;
    base.bg = s.bg;
    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            buf.set(x, y, base);

    // draw "[ label ]"
    int cx = rect.x;
    int cy = rect.y + rect.height / 2;
    int maxX = rect.x + rect.width;

    auto putChar = [&](char c) {
        if (cx >= maxX) return;
        backend::Cell cell;
        cell.ch        = c;
        cell.fg        = s.fg;
        cell.bg        = s.bg;
        cell.bold      = s.bold;
        cell.underline = s.underline;
        buf.set(cx++, cy, cell);
    };

    putChar('[');
    putChar(' ');
    for (unsigned char c : label) putChar(c);
    putChar(' ');
    putChar(']');
}

bool Button::onEvent(const core::Event& e) {
    if (e.type == core::EventType::Key) {
        if (e.key == utils::Key::Enter || e.key == ' ') {
            if (onClick) onClick();
            return true;
        }
    }
    return false;
}

} // namespace vextr::widgets