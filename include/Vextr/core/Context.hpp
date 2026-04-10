#pragma once
#include <Vextr/core/Event.hpp>
#include <Vextr/core/Geometry.hpp>
#include <memory>
#include <vector>

namespace vextr::core {

class Widget;

class FocusManager {
public:
  void setFocus(std::shared_ptr<Widget> w);
  void clearFocus();
  std::shared_ptr<Widget> focused() const { return current.lock(); }

  void focusNext(std::shared_ptr<Widget> root);
  void focusPrev(std::shared_ptr<Widget> root);

  bool dispatch(const Event &e, std::shared_ptr<Widget> root);

private:
  std::weak_ptr<Widget> current;

  void collectFocusable(std::shared_ptr<Widget> w,
                        std::vector<std::shared_ptr<Widget>> &out);
};

class Context {
public:
  static Context &get();

  Size terminalSize = {0, 0};
  FocusManager focusManager;
};

} // namespace vextr::core
