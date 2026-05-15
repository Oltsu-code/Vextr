#include <Vextr/backend/Renderer.hpp>
#include <Vextr/core/Context.hpp>
#include <Vextr/utils/Ansi.hpp>
#include <sstream>

namespace vextr::backend {

void Renderer::renderTree(core::Widget &root, Buffer &buf) {
  renderWidget(root, buf);
}

void Renderer::renderWidget(core::Widget &widget, Buffer &buf) {
  widget.render(buf);
  for (auto &child : widget.getChildren()) {
    renderWidget(*child, buf);
  }
}

void Renderer::renderOverlays(Buffer &buf) {
  for (auto &[widget, rect] : core::Context::get().overlayManager.stack()) {
    widget->render(buf);
    for (auto &child : widget->getChildren()) {
      renderWidget(*child, buf);
    }
  }
}

void Renderer::present(const Buffer &buf, Terminal &terminal) {
  std::ostringstream out;

  // cursor pos after last write (-1 = unknown)
  int cursorX = -1, cursorY = -1;

  for (int y = 0; y < buf.height(); ++y) {
    for (int x = 0; x < buf.width(); ++x) {
      const Cell &cell = buf.get(x, y);
      const Cell &prev = previousBuffer.get(x, y);

      // skip unchanged cells
      if (cell.ch == prev.ch &&
          cell.fg.r == prev.fg.r && cell.fg.g == prev.fg.g &&
          cell.fg.b == prev.fg.b &&
          cell.bg.transparent == prev.bg.transparent &&
          (cell.bg.transparent ||
           (cell.bg.r == prev.bg.r && cell.bg.g == prev.bg.g &&
            cell.bg.b == prev.bg.b)) &&
          cell.bold == prev.bold && cell.underline == prev.underline) {
        continue;
      }

      // only reposition if cursor isn't right after last write
      if (cursorX != x || cursorY != y)
        out << utils::ansi::cursor(y + 1, x + 1);

      // only emit style codes that changed
      bool attrChanged = (cell.bold != lastBold || cell.underline != lastUnderline);
      bool fgChanged = (cell.fg.r != lastFg.r || cell.fg.g != lastFg.g ||
                        cell.fg.b != lastFg.b);
      bool bgChanged = !cell.bg.transparent &&
                       (cell.bg.r != lastBg.r || cell.bg.g != lastBg.g ||
                        cell.bg.b != lastBg.b);

      if (attrChanged) {
        out << utils::ansi::reset();
        if (cell.bold)
          out << utils::ansi::bold();
        if (cell.underline)
          out << utils::ansi::underline();
        // reset clears colors too -> always re-emit
        out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
        if (!cell.bg.transparent)
          out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
        lastBold = cell.bold;
        lastUnderline = cell.underline;
        lastFg = cell.fg;
        lastBg = cell.bg;
      } else {
        if (fgChanged) {
          out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
          lastFg = cell.fg;
        }
        if (bgChanged) {
          out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
          lastBg = cell.bg;
        }
      }

      out << cell.ch;

      // at end of line terminal may auto-wrap -> treat cursor as unknown
      if (x < buf.width() - 1) {
        cursorX = x + 1;
        cursorY = y;
      } else {
        cursorX = -1;
        cursorY = -1;
      }
    }
  }

  if (!out.str().empty())
    terminal.write(out.str());

  previousBuffer.copyFrom(buf);
}

void Renderer::resize(int width, int height) {
  previousBuffer.resize(width, height);
  // terminal cleared on resize -> style state invalid
  lastFg = {255, 255, 255};
  lastBg = {0, 0, 0};
  lastBold = false;
  lastUnderline = false;
}

} // namespace vextr::backend
