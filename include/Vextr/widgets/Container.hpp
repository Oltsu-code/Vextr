#pragma once
#include <Vextr/core/Layout.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <memory>
#include <vector>

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
  bool onEvent(const Event &e) override;

  std::vector<std::shared_ptr<Widget>> getChildren() const override {
    std::vector<std::shared_ptr<Widget>> out;
    for (auto &slot : children)
      out.push_back(slot.widget);
    return out;
  }

protected:
  void drawContent(backend::Buffer &buf, Rect inner) override;

private:
  std::shared_ptr<Layout> layoutEngine;
  std::vector<ChildSlot> children;
};

} // namespace vextr::core