#pragma once

namespace vextr::core {

/// @struct Size
/// @brief Represents dimensions in cells.
///
/// Used to specify width and height, measured in terminal cells (characters).
struct Size {
  int width = 0;  ///< Width in cells
  int height = 0; ///< Height in cells
};

/// @struct Pos
/// @brief Represents a position in 2D space.
///
/// Coordinates are in terminal cells, with origin (0, 0) at top-left.
struct Pos {
  int x = 0; ///< X coordinate (column), 0-based
  int y = 0; ///< Y coordinate (row), 0-based
};

/// @struct Rect
/// @brief Represents a rectangular region in 2D space.
///
/// Defines position (x, y) and size (width, height) of a rectangular area.
/// Provides helper methods to calculate bounds.
struct Rect {
  int x = 0;      ///< X coordinate of top-left corner
  int y = 0;      ///< Y coordinate of top-left corner
  int width = 0;  ///< Width of rectangle
  int height = 0; ///< Height of rectangle

  /// @brief Returns the X coordinate of the right edge (exclusive).
  /// @return x + width
  int right() const { return x + width; }

  /// @brief Returns the Y coordinate of the bottom edge (exclusive).
  /// @return y + height
  int bottom() const { return y + height; }
};

} // namespace vextr::core