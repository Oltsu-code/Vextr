#pragma once
#include <Vextr/core/Widget.hpp>
#include <memory>
#include <vector>

namespace vextr::widgets {

/// @class ScrollView
/// @brief A clipping container that lets a child widget be scrolled inside a
/// fixed viewport.
///
/// ScrollView renders its child into an internal viewport and only copies the
/// visible region back to the parent buffer. This makes it useful for long
/// text blocks, logs, and any content that should be partially visible inside a
/// smaller area.
class ScrollView : public core::Widget {
public:
  ScrollView() = default;
  explicit ScrollView(std::shared_ptr<Widget> child);

  /// @brief Sets the widget to display inside the scroll view.
  /// @param child The content widget to scroll, or nullptr to clear it
  void setContent(std::shared_ptr<Widget> child);

  /// @brief Gets the current child widget.
  /// @return The scroll view content, or nullptr if empty
  std::shared_ptr<Widget> getContent() const { return content; }

  /// @brief Sets an explicit content size for the scrollable area.
  ///
  /// Use this when the content widget's natural measurement does not reflect
  /// its full scrollable size, such as when wrapping a container or layout.
  /// Passing 0 for a dimension auto-fills that dimension from the viewport.
  /// @param size The scrollable content size in cells
  void setContentSize(core::Size size);

  /// @brief Clears any explicit content size and falls back to measure().
  void clearContentSize();

  /// @brief Gets the horizontal scroll offset.
  int scrollX() const { return scroll.x; }

  /// @brief Gets the vertical scroll offset.
  int scrollY() const { return scroll.y; }

  /// @brief Sets the scroll offset, clamped to the current content bounds.
  void setScroll(int x, int y);

  /// @brief Scrolls relative to the current position.
  void scrollBy(int dx, int dy);

  bool isFocusable() const override { return true; }
  core::Size measure(int availW, int availH) override;
  void layout(int x, int y, int width, int height) override;
  bool onEvent(const core::Event &event) override;

  std::vector<std::shared_ptr<Widget>> getChildren() const override {
    // ScrollView renders its content through an internal clipped viewport, so
    // it must stay a render leaf to avoid the renderer drawing the child again.
    return {};
  }

  std::vector<std::shared_ptr<Widget>> getFocusChildren() const override {
    if (!content)
      return {};
    return {content};
  }

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  void clampScroll();
  void relayoutContent();
  core::Size resolveContentSize(core::Rect inner) const;

  std::shared_ptr<Widget> content;
  bool hasExplicitContentSize = false;
  core::Size explicitContentSize{};
  core::Size currentContentSize{};
  core::Rect currentInner{};
  bool hasLayout = false;
  core::Pos scroll{};
};

} // namespace vextr::widgets
