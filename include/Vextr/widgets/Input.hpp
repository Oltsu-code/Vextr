#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

/// @class Input
/// @brief A text input widget for user text entry.
///
/// Input provides a text field where users can type, edit, and submit text.
/// It supports standard keyboard editing (arrow keys, backspace, delete, etc.)
/// and displays a placeholder when empty.
///
/// **Features:**
/// - Text editing with cursor
/// - Placeholder text when empty
/// - onChange callback when text changes
/// - onSubmit callback when user presses Enter
/// - Horizontal scrolling for long text
///
/// @example
/// ```cpp
/// auto input = std::make_shared<Input>("Enter name...");
/// input->setOnChange([](const std::string &text) {
///   std::cout << "User typing: " << text << "\n";
/// });
/// input->setOnSubmit([](const std::string &text) {
///   std::cout << "User submitted: " << text << "\n";
/// });
/// ```
class Input : public core::Widget {
public:
  /// @brief Creates an input field with optional placeholder text.
  /// @param placeholder The text to show when input is empty (default: empty)
  explicit Input(const std::string &placeholder = "")
      : placeholder(placeholder) {}

  /// @brief Sets the input field's text.
  /// Moves the cursor to the end of the text.
  /// @param t The text to set
  void setText(const std::string &t) {
    text = t;
    cursor = (int)t.size();
  }

  /// @brief Gets the current text in the input field.
  /// @return The current text
  const std::string &getText() const { return text; }

  /// @brief Sets the placeholder text shown when the input is empty.
  /// @param p The placeholder text
  void setPlaceholder(const std::string &p) { placeholder = p; }

  /// @brief Sets the callback invoked when text changes.
  /// Called after each character insertion or deletion.
  /// @param cb Function receiving the current text (can be empty)
  void setOnChange(std::function<void(const std::string &)> cb) {
    onChange = std::move(cb);
  }

  /// @brief Sets the callback invoked when user presses Enter.
  /// Typically used to submit or confirm the input.
  /// @param cb Function receiving the final text (can be empty)
  void setOnSubmit(std::function<void(const std::string &)> cb) {
    onSubmit = std::move(cb);
  }

  /// @brief Returns true - input fields can receive focus.
  bool isFocusable() const override { return true; }

  /// @brief Measures the input field's size.
  core::Size measure(int availW, int availH) override;

  /// @brief Handles keyboard input and cursor movement.
  bool onEvent(const core::Event &e) override;

protected:
  /// @brief Renders the input field with text and cursor.
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string text;
  std::string placeholder;
  int cursor = 0; // cursor position in text
  int scroll = 0; // horizontal scroll offset

  std::function<void(const std::string &)> onChange;
  std::function<void(const std::string &)> onSubmit;

  void insertChar(char c);
  void deleteBack();
  void deleteForward();
  void moveCursor(int delta);
  void updateScroll();
};

} // namespace vextr::widgets