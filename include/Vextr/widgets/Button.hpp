#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

/// @class Button
/// @brief A clickable button widget.
///
/// Button displays text and responds to click events. It's focusable and can be
/// activated via Enter key or mouse click. Use setOnClick() to handle
/// activation.
///
/// @example
/// ```cpp
/// auto btn = std::make_shared<Button>("Click me");
/// btn->setOnClick([]() {
///   std::cout << "Button clicked!\n";
/// });
/// ```
class Button : public core::Widget {
public:
  /// @brief Creates a button with optional label text.
  /// @param label The text to display on the button (default: empty)
  explicit Button(const std::string &label = "") : label(label) {}

  /// @brief Sets the button's label text.
  /// @param l The new label text
  void setLabel(const std::string &l) { label = l; }

  /// @brief Gets the button's label text.
  /// @return The current label
  const std::string &getLabel() const { return label; }

  /// @brief Sets the callback to invoke when the button is activated.
  /// The callback is called when the user presses Enter (if focused) or clicks.
  /// @param cb The function to call on activation (can be empty)
  void setOnClick(std::function<void()> cb) { onClick = std::move(cb); }

  /// @brief Returns true - buttons can receive focus.
  bool isFocusable() const override { return true; }

  /// @brief Measures the button's size based on label text.
  core::Size measure(int availW, int availH) override;

  /// @brief Handles key press and mouse click events.
  bool onEvent(const core::Event &e) override;

protected:
  /// @brief Renders the button's text content.
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string label;
  std::function<void()> onClick;
};

} // namespace vextr::widgets