#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::core {

/// @class GridLayout
/// @brief A layout that arranges children in a 2D grid.
///
/// GridLayout divides the available space into a fixed grid of rows and
/// columns. Children position themselves using the col, row, colSpan, and
/// rowSpan fields in their LayoutSpec.
///
/// @example
/// ```cpp
/// // Create a 3x2 grid (3 columns, 2 rows) with 1 cell spacing
/// auto grid = std::make_shared<Container>(
///   std::make_shared<GridLayout>(3, 2, 1, 1)
/// );
///
/// // Add widgets specifying their grid position
/// auto widget1 = std::make_shared<Label>("Top-left");
/// auto widget2 = std::make_shared<Label>("Top-right");
/// auto widget3 = std::make_shared<Label>("Bottom (spans 3 cols)");
///
/// grid->addChild(widget1, LayoutSpec{.col = 0, .row = 0});
/// grid->addChild(widget2, LayoutSpec{.col = 2, .row = 0});
/// grid->addChild(widget3, LayoutSpec{.col = 0, .row = 1, .colSpan = 3});
/// ```
class GridLayout : public Layout {
public:
  /// @brief Creates a grid layout.
  /// @param cols Number of columns
  /// @param rows Number of rows
  /// @param gapX Horizontal spacing between columns in cells (default: 0)
  /// @param gapY Vertical spacing between rows in cells (default: 0)
  GridLayout(int cols, int rows, int gapX = 0, int gapY = 0)
      : cols(cols), rows(rows), gapX(gapX), gapY(gapY) {}

  /// @brief Applies the grid layout.
  void apply(std::vector<ChildSlot> &children, Rect inner) override;

private:
  int cols, rows, gapX, gapY;
};

} // namespace vextr::core
