#pragma once
#include <Vextr/core/FocusManager.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/OverlayManager.hpp>

namespace vextr::core {

/// @class Context
/// @brief Centralized application context holding global state and managers
class Context {
public:
  /// @brief Access the global context instance
  static Context &get();

  Size terminalSize = {0, 0};
  FocusManager focusManager;
  OverlayManager overlayManager;
};

} // namespace vextr::core
