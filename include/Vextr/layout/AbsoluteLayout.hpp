#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::layout {

/// @class AbsoluteLayout
/// @brief A layout where children are positioned using absolute coordinates.
///
/// AbsoluteLayout allows precise control over widget positioning. Each child's
/// position is determined by the absX and absY fields in its LayoutSpec.
///
/// **Advantages:**
/// - Precise control over positioning
/// - No automatic resizing based on available space
///
/// **Disadvantages:**
/// - Not responsive to terminal resizing
/// - Widgets can overlap
/// - Less maintainable for complex layouts
///
/// @example
/// ```cpp
/// auto abs = std::make_shared<Container>(
///   std::make_shared<AbsoluteLayout>()
/// );
///
/// auto button1 = std::make_shared<Button>("Top-left");
/// auto button2 = std::make_shared<Button>("Bottom-right");
///
/// // Position at (5, 3)
/// abs->addChild(button1, LayoutSpec{.absX = cells(5), .absY = cells(3)});
///
/// // Position at (20, 15)
/// abs->addChild(button2, LayoutSpec{.absX = cells(20), .absY = cells(15)});
/// ```
class AbsoluteLayout : public core::Layout {
public:
  /// @brief Applies absolute positioning to children.
  void apply(std::vector<core::ChildSlot> &children, core::Rect inner) override;
};

} // namespace vextr::layout