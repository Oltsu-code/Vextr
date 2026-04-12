#pragma once
#include <Vextr/core/Widget.hpp>
#include <string>

namespace vextr::widgets {

/// @class Label
/// @brief A read-only text display widget.
///
/// Label displays static text and is not focusable. It's useful for titles,
/// descriptions, and other informational text. The label automatically sizes
/// itself based on its text.
///
/// @example
/// ```cpp
/// auto label = std::make_shared<Label>("Hello, World!");
/// ```
class Label : public core::Widget {
public:
  /// @brief Creates an empty label.
  Label() = default;

  /// @brief Creates a label with initial text.
  /// @param text The text to display
  explicit Label(const std::string &text) : text(text) {}

  /// @brief Sets the label's text.
  /// @param t The new text to display
  void setText(const std::string &t) { text = t; }

  /// @brief Gets the label's text.
  /// @return The current text
  const std::string &getText() const { return text; }

  /// @brief Measures the label's size based on its text.
  core::Size measure(int availW, int availH) override;

protected:
  /// @brief Renders the label's text content.
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string text;
};

} // namespace vextr::widgets
