#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Align.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/Style.hpp>
#include <memory>
#include <vector>

namespace vextr::core {

class Widget : public std::enable_shared_from_this<Widget> {
public:
  virtual ~Widget() = default;

  virtual Size measure(int availW, int availH);
  virtual void layout(int x, int y, int width, int height);

  // overriding this will brake any border stuff and should only be done if you
  // know what youre doing
  virtual void render(backend::Buffer &buf);

  virtual bool onEvent(const Event &event) { return false; }
  virtual bool isFocusable() const { return false; }
  virtual void onFocus() {}
  virtual void onBlur() {}

  bool focused() const;

  void setStyle(const Style &s) { style = s; }
  void setFocusedStyle(const Style &s) { focusedStyle = s; }
  const Style &getStyle() const { return style; }
  const Style &getFocusedStyle() const { return focusedStyle; }
  const Rect &getRect() const { return rect; }

  virtual std::vector<std::shared_ptr<Widget>> getChildren() const {
    return {};
  }

  std::weak_ptr<Widget> parent;

protected:
  virtual void drawContent(backend::Buffer &buf, Rect inner) {}

  const Style &activeStyle() const;
  Rect innerRect() const;

  Style style;
  Style focusedStyle;
  Rect rect;

  int justifyX(int innerX, int innerW, int contentW, Align align) const;
  int justifyY(int innerY, int innerH, int contentH, Align align) const;

  Rect alignContentRect(Rect inner, int contentW, int contentH, Align ax,
                        Align ay) const;

private:
  void drawBackground(backend::Buffer &buf);
  void drawBorder(backend::Buffer &buf);
};

} // namespace vextr::core