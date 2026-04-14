#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/backend/Terminal.hpp>
#include <Vextr/core/Widget.hpp>

namespace vextr::backend {

class Renderer {
public:
  /// @brief Recursively renders widget tree into buffer
  void renderTree(core::Widget &root, Buffer &buf);

  /// @brief Renders all overlays on top of the main tree
  void renderOverlays(Buffer &buf);

  /// @brief Optimized terminal presentation with diff-based updates
  void present(const Buffer &buf, Terminal &terminal);

  /// @brief Resize internal buffers to match terminal size
  void resize(int width, int height);

private:
  void renderWidget(core::Widget &widget, Buffer &buf);

  Buffer previousBuffer{0, 0};
};

} // namespace vextr::backend
