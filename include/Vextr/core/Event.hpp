#pragma once

namespace vextr::core {

/// @enum EventType
/// @brief Enumeration of all possible input event types.
///
/// Used to distinguish between different kinds of input events that widgets can
/// receive.
enum class EventType {
  Key,          ///< Keyboard key press
  MousePress,   ///< Mouse button pressed
  MouseRelease, ///< Mouse button released
  MouseMove,    ///< Mouse pointer moved
  Resize,       ///< Terminal window resized
};

/// @enum MouseButton
/// @brief Mouse button identifier.
enum class MouseButton {
  None,   ///< No button (used in MouseMove events)
  Left,   ///< Left mouse button
  Right,  ///< Right mouse button
  Middle, ///< Middle mouse button
};

/// @struct Event
/// @brief Represents a single input event from the terminal.
///
/// An event can be a keyboard input, mouse action, or terminal resize.
/// Which fields are meaningful depends on the @c type field:
///
/// - **Key events**: @c key, @c shift, @c ctrl, @c alt
/// - **Mouse events**: @c mouseX, @c mouseY, @c button
/// - **Resize events**: @c newWidth, @c newHeight
///
/// @example
/// ```cpp
/// bool MyWidget::onEvent(const Event &e) {
///   if (e.type == EventType::Key && e.key == 'q') {
///     app->quit();
///     return true;
///   }
///   if (e.type == EventType::MousePress && e.button == MouseButton::Left) {
///     handleClick(e.mouseX, e.mouseY);
///     return true;
///   }
///   return false;
/// }
/// ```
struct Event {
  /// @brief The type of event.
  EventType type;

  /// @brief The key code (for EventType::Key).
  /// Key codes match standard ASCII where applicable.
  int key = 0;

  /// @brief True if Shift was held during key press.
  bool shift = false;

  /// @brief True if Ctrl was held during key press.
  bool ctrl = false;

  /// @brief True if Alt was held during key press.
  bool alt = false;

  /// @brief X coordinate of mouse event (0-based, relative to terminal).
  int mouseX = 0;

  /// @brief Y coordinate of mouse event (0-based, relative to terminal).
  int mouseY = 0;

  /// @brief Which mouse button was pressed/released.
  MouseButton button = MouseButton::None;

  /// @brief New terminal width (for EventType::Resize).
  int newWidth = 0;

  /// @brief New terminal height (for EventType::Resize).
  int newHeight = 0;
};

} // namespace vextr::core
