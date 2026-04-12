#pragma once
#include <Vextr/core/Layout.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <memory>
#include <vector>

namespace vextr::widgets {

/// @class Container
/// @brief A widget that arranges child widgets using a layout engine.
///
/// Container is the primary way to organize multiple widgets. It holds children
/// and uses a Layout to position them. By default, it uses VerticalStackLayout,
/// but you can specify any Layout (Grid, Absolute, custom, etc.).
///
/// Container is not focusable itself - focus passes through to focusable
/// children.
///
/// **Usage patterns:**
/// ```cpp
/// // Default vertical stack
/// auto container = std::make_shared<Container>();
/// container->addChild(std::make_shared<Label>("Title"));
/// container->addChild(std::make_shared<Button>("Click me"));
///
/// // Horizontal stack
/// auto hbox = std::make_shared<Container>(
///   std::make_shared<StackLayout>(Axis::Horizontal)
/// );
///
/// // Grid layout
/// auto grid = std::make_shared<Container>(
///   std::make_shared<GridLayout>(2, 3) // 2 cols, 3 rows
/// );
/// ```
class Container : public core::Widget {
public:
  /// @brief Creates a container with the specified layout.
  /// @param layout The layout engine to use (default: VerticalStackLayout)
  explicit Container(
      std::shared_ptr<core::Layout> layout =
          std::make_shared<layout::StackLayout>(layout::Axis::Vertical))
      : layoutEngine(std::move(layout)) {}

  /// @brief Adds a child widget to the container.
  ///
  /// The LayoutSpec controls how the child is sized and positioned within its
  /// slot.
  ///
  /// @param child The widget to add
  /// @param spec Layout specification for this child (optional)
  ///
  /// @example
  /// ```cpp
  /// auto container = std::make_shared<Container>();
  /// auto button = std::make_shared<Button>("Flex");
  /// container->addChild(button, LayoutSpec{.ratio = 2.0f}); // Double weight
  /// ```
  void addChild(std::shared_ptr<Widget> child, core::LayoutSpec spec = {});

  /// @brief Changes the layout engine.
  /// @param l The new layout to use
  void setLayout(std::shared_ptr<core::Layout> l) { layoutEngine = std::move(l); }

  /// @brief Measures the container's size (sum of children).
  core::Size measure(int availW, int availH) override;

  /// @brief Lays out all children using the layout engine.
  void layout(int x, int y, int width, int height) override;

  /// @brief Passes events to focusable children.
  bool onEvent(const core::Event &e) override;

  /// @brief Returns all child widgets.
  std::vector<std::shared_ptr<Widget>> getChildren() const override {
    std::vector<std::shared_ptr<Widget>> out;
    for (auto &slot : children)
      out.push_back(slot.widget);
    return out;
  }

protected:
  /// @brief Draws the container's content (children are drawn by the
  /// framework).
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::shared_ptr<core::Layout> layoutEngine;
  std::vector<core::ChildSlot> children;
};

} // namespace vextr::core