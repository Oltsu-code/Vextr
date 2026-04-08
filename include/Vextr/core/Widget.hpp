#pragma once
#include <Vextr/core/Style.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Event.hpp>

namespace vextr::core {

class Widget {
public:
    virtual ~Widget() = default;

    virtual Size measure(int availW, int availH);
    virtual void layout(int x, int y, int width, int height);
    virtual void render(backend::Buffer& buf);
    virtual bool onEvent(const Event& event) { return false; }
    virtual bool isFocusable() const { return false; }

    void setStyle(const Style& s) { style = s; }
    const Style& getStyle() const { return style; }
    const Rect& getRect() const { return rect; }

protected:
    Style style;
    Rect  rect;
};

} // vextr::core

