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
  out << utils::ansi::cursor(1, 1);

  core::Color lastFg = {255, 255, 255};
  core::Color lastBg = {0, 0, 0};
  bool lastBold = false;
  bool lastUnderline = false;

  for (int y = 0; y < buf.height(); ++y) {
    for (int x = 0; x < buf.width(); ++x) {
      const Cell &cell = buf.get(x, y);

      // Check if attributes changed
      bool attrChanged =
          (cell.bold != lastBold || cell.underline != lastUnderline);
      bool fgChanged = (cell.fg.r != lastFg.r || cell.fg.g != lastFg.g ||
                        cell.fg.b != lastFg.b);
      bool bgChanged = false;
      if (!cell.bg.transparent) {
        bgChanged = (cell.bg.r != lastBg.r || cell.bg.g != lastBg.g ||
                     cell.bg.b != lastBg.b);
      }

      // emit style changes only when needed
      if (attrChanged) {
        out << utils::ansi::reset();
        if (cell.bold)
          out << utils::ansi::bold();
        if (cell.underline)
          out << utils::ansi::underline();
        // After reset, always re-emit colors
        out << utils::ansi::fg_rgb(cell.fg.r, cell.fg.g, cell.fg.b);
        if (!cell.bg.transparent) {
          out << utils::ansi::bg_rgb(cell.bg.r, cell.bg.g, cell.bg.b);
        }
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
    }
    if (y < buf.height() - 1)
      out << '\n';
  }

  terminal.write(out.str());
  previousBuffer.copyFrom(buf);
}

void Renderer::resize(int width, int height) {
  previousBuffer.resize(width, height);
}

} // namespace vextr::backend
