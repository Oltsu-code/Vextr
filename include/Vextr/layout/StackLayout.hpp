#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::layout {

/// @enum Axis
/// @brief Specifies the direction for stack-based layout.
enum class Axis {
  Horizontal, ///< Stack children left-to-right
  Vertical,   ///< Stack children top-to-bottom
};

/// @class StackLayout
/// @brief A layout that stacks children in a single row or column.
///
/// StackLayout arranges children either horizontally or vertically.
/// Each child's size is determined by:
/// 1. Its fixed size (if specified via LayoutSpec)
/// 2. Its ratio weight (if dividing remaining space)
///
/// **Features:**
/// - Vertical (default) or horizontal arrangement
/// - Flexible sizing with ratio weights
/// - Fixed sizing with cells() or pct()
/// - Spacing between children (gap)
///
/// @example
/// ```cpp
/// // Vertical stack (default)
/// auto vstack = std::make_shared<Container>(
///   std::make_shared<StackLayout>(Axis::Vertical, 1) // 1 cell gap
/// );
///
/// // Horizontal stack
/// auto hstack = std::make_shared<Container>(
///   std::make_shared<StackLayout>(Axis::Horizontal, 2)
/// );
/// ```
class StackLayout : public core::Layout {
public:
  /// @brief Creates a stack layout.
  /// @param axis The direction to stack (default: Vertical)
  /// @param gap Spacing between children in cells (default: 0)
  explicit StackLayout(Axis axis = Axis::Vertical, int gap = 0)
      : axis(axis), gap(gap) {}

  /// @brief Applies the layout by stacking children along the axis.
  void apply(std::vector<core::ChildSlot> &children, core::Rect inner) override;

private:
  Axis axis;
  int gap;

  core::Rect resolveSlot(const core::ChildSlot &slot, int x, int y, int w,
                         int h);
};

} // namespace vextr::layout