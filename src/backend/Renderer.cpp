#include <sstream>
#include <Vextr/backend/Renderer.hpp>
#include <Vextr/utils/Ansi.hpp>

namespace vextr::backend {

void Renderer::render(core::Widget& root, Buffer& buf) {
    root.render(buf);
}

void Renderer::present(const Buffer& buf, Terminal& terminal)
{
    std::ostringstream out;
    out << utils::ansi::cursor(1, 1);

    core::Color lastFg = {255,255,255};
    core::Color lastBg = {0,0,0};
    bool lastBold = false;
    bool lastUnderline = false;

    for (int y = 0; y < buf.height(); ++y) {
        for (int x = 0; x < buf.width(); ++x) {
            const Cell& cell = buf.get(x,y);

            if (cell.fg.r != lastFg.r || cell.fg.g != lastFg.g || cell.fg.b != lastFg.b) {
                out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
                lastFg = cell.fg;
            }

            if (cell.bg.r != lastBg.r || cell.bg.g != lastBg.g || cell.bg.b != lastBg.b) {
                out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
                lastBg = cell.bg;
            }

            if (cell.bold != lastBold || cell.underline != lastUnderline) {
                out << utils::ansi::reset();
                if (cell.bold) out << utils::ansi::bold();
                if (cell.underline) out << utils::ansi::underline();

                out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
                out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);

                lastBold = cell.bold;
                lastUnderline = cell.underline;
            }

            out << cell.ch;
        }

        if (y < buf.height() - 1)
            out << '\n';
    }

    terminal.write(out.str());
}

} // vextr::backend
