#pragma once
#include <Vextr/core/Event.hpp>
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

  bool dispatchToFocused(const Event &e);
  bool dispatch(const Event &e, std::shared_ptr<Widget> root);

  void focusTopOverlay();

private:
  std::weak_ptr<Widget> current;

  void collectFocusable(std::shared_ptr<Widget> w,
                        std::vector<std::shared_ptr<Widget>> &out);
};

} // namespace vextr::core