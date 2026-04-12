#pragma once
#include <Vextr/core/FocusManager.hpp>
#include <Vextr/core/Geometry.hpp>

namespace vextr::core {

class Context {
public:
  static Context &get();

  Size terminalSize = {0, 0};
  FocusManager focusManager;
};

} // namespace vextr::core
