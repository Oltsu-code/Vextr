#pragma once
#include <Vextr/core/Widget.hpp>
#include <Vextr/widgets/TextFlow.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

/// @class TextArea
/// @brief A multiline text input widget.
///
/// TextArea supports editing text across multiple lines with cursor movement,
/// insertion, deletion, and vertical/horizontal scrolling.
class TextArea : public core::Widget {
public:
  /// @brief Creates a TextArea with optional placeholder text.
  /// @param placeholder The text to show when the TextArea is empty (default:
  /// empty
  explicit TextArea(const std::string &placeholder = "")
      : placeholder(placeholder) {}

  /// @brief Set the text of the TextArea.
  /// @param t The new text content
  void setText(const std::string &t);

  /// @brief Get the current text of the TextArea.
  /// @return The current text content
  const std::string &getText() const { return text; }

  /// @brief Set the placeholder text shown when the TextArea is empty.
  /// @param p The new placeholder text
  void setPlaceholder(const std::string &p) { placeholder = p; }

  /// @brief Set the text flow mode, which determines how the TextArea handles
  /// overflow.
  /// @param mode The new TextFlowMode (default: Scroll)
  void setFlowMode(TextFlowMode mode) { flowMode = mode; }

  /// @brief Get the current text flow mode.
  /// @return The current flow mode
  TextFlowMode getFlowMode() const { return flowMode; }

  /// @brief Set what happens when the text changes. The callback is called with
  /// the new text whenever it changes.
  /// @param cb The callback function to call on text change
  void setOnChange(std::function<void(const std::string &)> cb) {
    onChange = std::move(cb);
  }

  bool isFocusable() const override { return true; }
  core::Size measure(int availW, int availH) override;
  bool onEvent(const core::Event &e) override;

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string text;
  std::string placeholder;
  int cursor = 0;
  int preferredColumn = -1;
  int scrollX = 0;
  int scrollY = 0;
  TextFlowMode flowMode = TextFlowMode::Scroll;

  std::function<void(const std::string &)> onChange;

  void insertChar(char c);
  void insertNewline();
  void deleteBack();
  void deleteForward();
  void moveCursorHorizontal(int delta);
  void moveCursorVertical(int delta);
  void moveToLineBoundary(bool endOfLine);
  void updateScroll();
};

} // namespace vextr::widgets
