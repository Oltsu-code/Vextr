#pragma once
#include <Vextr/core/Widget.hpp>
#include <Vextr/widgets/TextFlow.hpp>
#include <string>

namespace vextr::widgets {

/// @class TextView
/// @brief A read-only multiline text widget.
///
/// TextView displays text over multiple lines and supports optional vertical
/// scrolling for long content.
class TextView : public core::Widget {
public:
  TextView() = default;

  /// @brief Create a text view with text inside it
  /// @param text The text to display in the TextView
  explicit TextView(const std::string &text) : text(text) {}

  /// @brief Set the text content of the TextView.
  /// @param t The new text content
  void setText(const std::string &t) { text = t; }

  /// @brief Get the current text content of the TextView.
  /// @return The current text content
  const std::string &getText() const { return text; }

  /// @brief Set the text flow mode, which determines how the TextArea handles
  /// overflow.
  /// @param mode The new TextFlowMode (default: Scroll)
  void setFlowMode(TextFlowMode mode) { flowMode = mode; }

  /// @brief Get the current text flow mode.
  /// @return The current flow mode
  TextFlowMode getFlowMode() const { return flowMode; }

  bool isFocusable() const override;
  bool onEvent(const core::Event &event) override;

  void setScrollX(int x);
  int scrollX() const { return scrollOffsetX; }
  void setScrollY(int y);
  int scrollY() const { return scrollOffsetY; }

  core::Size measure(int availW, int availH) override;

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string text;
  int scrollOffsetX = 0;
  int scrollOffsetY = 0;
  TextFlowMode flowMode = TextFlowMode::Scroll;
};

} // namespace vextr::widgets
