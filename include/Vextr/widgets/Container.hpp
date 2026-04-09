#pragma once
#include <Vextr/core/Widget.hpp>
#include <Vextr/core/Layout.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <vector>
#include <memory>

namespace vextr::core {

class Container : public Widget {
public:
    explicit Container(std::shared_ptr<Layout> layout =
        std::make_shared<StackLayout>(Axis::Vertical))
        : layoutEngine(std::move(layout)) {}

    void addChild(std::shared_ptr<Widget> child, LayoutSpec spec = {});
    void setLayout(std::shared_ptr<Layout> l) { layoutEngine = std::move(l); }

    Size measure(int availW, int availH) override;
    void layout(int x, int y, int width, int height) override;
    void render(backend::Buffer& buf) override;
    bool onEvent(const Event& e) override;

private:
    std::shared_ptr<Layout> layoutEngine;
    std::vector<ChildSlot>  children;
};

} // vextr::core