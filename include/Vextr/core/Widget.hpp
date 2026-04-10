#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/Style.hpp>
#include <memory>

namespace vextr::core {

class Widget : public std::enable_shared_from_this<Widget> {
public:
  virtual ~Widget() = default;

  virtual Size measure(int availW, int availH);
  virtual void layout(int x, int y, int width, int height);
  virtual void render(backend::Buffer &buf);
  virtual bool onEvent(const Event &event) { return false; }
  virtual bool isFocusable() const { return false; }
  virtual void onFocus() {}
  virtual void onBlur() {}

  virtual std::vector<std::shared_ptr<Widget>> getChildren() const {
    return {};
  }

  bool focused() const;

  void setStyle(const Style &s) { style = s; }
  void setFocusedStyle(const Style s) { focusedStyle = s; }
  const Style &getStyle() const { return style; }
  const Style &getFocusedStyle() const { return focusedStyle; }
  const Rect &getRect() const { return rect; }

  std::weak_ptr<Widget> parent;

protected:
  const Style &activeStyle() const;

  Style style;
  Style focusedStyle;
  Rect rect;
};

} // namespace vextr::core
