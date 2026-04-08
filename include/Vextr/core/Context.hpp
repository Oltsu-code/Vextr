#pragma once
#include <Vextr/core/Geometry.hpp>
#include <memory>

namespace vextr::core {

class Widget;

class Context {
public:
    static Context& get();

    Size terminalSize = {0, 0};
    std::weak_ptr<Widget> focusedWidget;

    void setFocus(std::shared_ptr<Widget> w) { focusedWidget = w; }
    std::shared_ptr<Widget> getFocus() { return focusedWidget.lock(); }
};

} // vextr::core
