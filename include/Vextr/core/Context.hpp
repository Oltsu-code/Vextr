#pragma once
#include <Vextr/core/FocusManager.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/OverlayManager.hpp>

namespace vextr::core {

class Context {
public:
  static Context &get();

  Size terminalSize = {0, 0};
  FocusManager focusManager;
  OverlayManager overlayManager;
};

} // namespace vextr::core
