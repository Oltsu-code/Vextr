#pragma once
#include <Vextr/core/Geometry.hpp>
#include <memory>
#include <vector>

namespace vextr::core {

class Widget;

/// @class OverlayManager
/// @brief Manages floating widgets that render above the main widget tree.
///
/// Overlays are used for dropdowns, tooltips, modals, and other popups
/// that need to escape their parent's bounds. They are rendered after
/// the main tree so they always appear on top.
class OverlayManager {
public:
  /// @brief Pushes a widget as an overlay at the given screen rect.
  /// @param widget The widget to display as an overlay
  /// @param rect The screen position and size to render it at
  void push(std::shared_ptr<Widget> widget, Rect rect);

  /// @brief Removes the topmost overlay.
  void pop();

  /// @brief Removes all overlays.
  void clear();

  /// @brief Relayouts all overlays for new terminal size.
  void resize(int termWidth, int termHeight);

  /// @brief Returns true if there are any active overlays.
  bool hasOverlays() const { return !overlays.empty(); }

  /// @brief Returns the overlay stack (bottom to top).
  const std::vector<std::pair<std::shared_ptr<Widget>, Rect>> &stack() const {
    return overlays;
  }

  std::vector<std::pair<std::shared_ptr<Widget>, Rect>> &stack() {
    return overlays;
  }

private:
  std::vector<std::pair<std::shared_ptr<Widget>, Rect>> overlays;
};

} // namespace vextr::core
