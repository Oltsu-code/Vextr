#pragma once
#include <Vextr/core/Event.hpp>
#include <memory>
#include <vector>

namespace vextr::core {

class Widget;

/// @class FocusManager
/// @brief Manages focus among widgets, including traversal and event dispatching
class FocusManager {
public:
  /// @brief Set focus to any widget
  /// @param w The widget to receive focus.
  void setFocus(std::shared_ptr<Widget> w);

  /// @brief Clear focus from any widget
  void clearFocus();

  /// @brif Get the currently focused widget, if any
  std::shared_ptr<Widget> focused() const { return current.lock(); }

  /// @brief Move focus to the next focusable widget in the tree
  /// @param root The root widget for focus traversal
  void focusNext(std::shared_ptr<Widget> root);

  /// @brief Move focus to the previous focusable widget in the tree
  /// @param root The root widget for focus traversal
  void focusPrev(std::shared_ptr<Widget> root);

  /// @brief Dispatch an event to the currently focused widget, if any
  /// @param e The event to dispatch
  bool dispatchToFocused(const Event &e);

  /// @brief Dispatch an event
  /// This also includes handling of global focus-related keys like Tab and Escape.
  ///
  /// @param e The event to dispatch
  /// @param root The root widget for focus traversal if needed (e.g. on Tab key))
  bool dispatch(const Event &e, std::shared_ptr<Widget> root);

  /// @brief Focus the top overlay from the overlay stack
  void focusTopOverlay();

private:
  /// @brief The currently focused Widget
  std::weak_ptr<Widget> current;

  /// @brief Collect the focusable widgets
  /// Recursively traverses the widget tree to find all focusable widgets and adds them to the output vector.
  ///
  /// @param w The widget to start traversal from
  /// @param out The output vector to collect focusable widgets into
  void collectFocusable(std::shared_ptr<Widget> w,
                        std::vector<std::shared_ptr<Widget>> &out);
};

} // namespace vextr::core