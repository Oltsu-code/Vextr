#pragma once
#include <variant>

namespace vextr::core::events {

/// @struct KeyModifiers
/// @brief Keyboard modifier keys held during a key event
struct KeyModifiers {
  bool shift = false;
  bool ctrl = false;
  bool alt = false;
};

/// @enum MouseButton
/// @brief Mouse button identifier
enum class MouseButton {
  None,   ///< No button (used in MouseMove events)
  Left,   ///< Left mouse button
  Right,  ///< Right mouse button
  Middle, ///< Middle mouse button
};

/// @struct KeyEvent
/// @brief A keyboard key press event.
///
/// Key codes match standard ASCII where applicable. Special keys use
/// values from @c vextr::utils::Key.
///
/// **Example:**
/// ```cpp
/// if (auto* k = std::get_if<KeyEvent>(&e)) {
///   if (k->key == 'q' && !k->modifiers.ctrl)
///     app->quit();
/// }
/// ```
struct KeyEvent {
  int key;                ///< Key code (ASCII or utils::Key value)
  KeyModifiers modifiers; ///< Modifier keys held during the press
};

/// @struct MousePressEvent
/// @brief A mouse button press event.
/// @note This has not been implemented yet!
struct MousePressEvent {
  int x, y;           ///< Cursor position (0-based, relative to terminal)
  MouseButton button; ///< Which button was pressed
};

/// @struct MouseReleaseEvent
/// @brief A mouse button release event
/// @note This has not been implemented yet!
struct MouseReleaseEvent {
  int x, y;           ///< Cursor position (0-based, relative to terminal)
  MouseButton button; ///< Which button was released
};

/// @struct MouseMoveEvent
/// @brief A mouse move event (no button pressed)
/// @note This has not been implemented yet!
struct MouseMoveEvent {
  int x, y; ///< New cursor position (0-based, relative to terminal)
};

/// @struct ResizeEvent
/// @brief A terminal resize event
struct ResizeEvent {
  int width, height; ///< New terminal dimensions in columns and rows
};

/// @struct Event
/// @brief A tagged union of all possible input events.
///
/// Use @c std::get_if to check the active type and access its fields:
/// ```cpp
/// bool Widget::onEvent(const Event& e) {
///   if (auto* k = std::get_if<KeyEvent>(&e)) {
///     // handle key
///   }
///   return false;
/// }
/// ```
struct Event : std::variant<KeyEvent, MousePressEvent, MouseReleaseEvent,
                            MouseMoveEvent, ResizeEvent> {
  using variant::variant; // inherit constructors
};

/// @brief Helper for exhaustive std::visit with multiple lambdas.
///
/// **Example:**
/// ```cpp
/// std::visit(overloaded{
///   [](const KeyEvent& k)        { /* ... */ },
///   [](const MousePressEvent& m) { /* ... */ },
///   [](const auto&)              { /* catch-all */ }
/// }, event);
/// ```
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

} // namespace vextr::core::events
