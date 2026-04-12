#pragma once
#include <Vextr/core/Align.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace vextr::core {

/// @struct Color
/// @brief Represents an RGB color with optional transparency.
///
/// Colors are 24-bit RGB, allowing any of 16 million colors.
/// Can also be marked as transparent to use the terminal's default background.
struct Color {
  uint8_t r = 255;          ///< Red channel (0-255)
  uint8_t g = 255;          ///< Green channel (0-255)
  uint8_t b = 255;          ///< Blue channel (0-255)
  bool transparent = false; ///< If true, this color uses the terminal's default

  /// @brief Creates a transparent color that uses the terminal's default.
  /// @return A transparent Color
  static Color clear() { return {0, 0, 0, true}; }
};

/// @struct TextDecoration
/// @brief Represents text styling options such as bold, underline, and italic.
///
/// These styles can be combined to achieve various text effects.
struct TextDecoration {
  bool bold = false;
  bool underline = false;
  bool italic = false;
  bool strikethrough = false;
};

/// @struct PaddingSpec
/// @brief Specifies padding on all four sides of a widget's content area.
///
/// Padding is the space between a widget's border and its content.
/// All values are in cells (characters) — percentage-based padding is not
/// supported since it would require knowing the widget's final size in advance.
///
/// @example
/// ```cpp
/// style.padding = PaddingSpec::all(1);          // 1 cell on all sides
/// style.padding = PaddingSpec::xy(2, 1);        // 2 left/right, 1 top/bottom
/// style.padding = {.top = 1, .bottom = 1};      // manual per-side
/// ```
struct PaddingSpec {
  int top = 0;    ///< Padding on the top edge (in cells)
  int bottom = 0; ///< Padding on the bottom edge (in cells)
  int left = 0;   ///< Padding on the left edge (in cells)
  int right = 0;  ///< Padding on the right edge (in cells)

  /// @brief Creates equal padding on all four sides.
  /// @param v The number of cells to pad on each side
  static PaddingSpec all(int v) { return {v, v, v, v}; }

  /// @brief Creates padding with separate horizontal and vertical values.
  /// @param x Padding on the left and right edges (in cells)
  /// @param y Padding on the top and bottom edges (in cells)
  static PaddingSpec xy(int x, int y) { return {y, y, x, x}; }

  /// @brief Creates zero padding on all sides.
  static PaddingSpec none() { return {}; }
};

/// @enum BorderStyle
/// @brief Specifies the style of border to draw around widgets.
///
/// Different border styles use different Unicode box-drawing characters.
enum class BorderStyle {
  None,    ///< No border
  Single,  ///< Single-line border: ─ │ ┌ ┐ └ ┘
  Double,  ///< Double-line border: ═ ║ ╔ ╗ ╚ ╝
  Rounded, ///< Rounded corners: ─ │ ╭ ╮ ╰ ╯
  Thick,   ///< Thick border: ━ ┃ ┏ ┓ ┗ ┛
  Dashed,  ///< Dashed border: ╌ ╎
};

///@struct BorderLabel
///@brief Represents a label that can be displayed on a border edge of a widget.
///
/// Border labels are optional text elements that can be placed on the top or bottom.
/// They can be aligned to any position (start, center, end) and can have their own text decoration and color.
struct BorderLabel {
  std::string text; ///< The text to display on the border
  Align align = Align::Center; ///< Alignment (start, center, end) of the label on the border

  TextDecoration textDecoration; /// Optional text decoration for the label (bold, underline, etc.)
  Color color; /// Optional color for the label text
};

/// @struct Border
/// @brief Describes the border styling for a widget.
struct Border {
  BorderStyle style = BorderStyle::None;     ///< The border style
  Color color = {255, 255, 255}; ///< The border color

  std::vector<BorderLabel> title;     /// Optional labels to display on the top border
  std::vector<BorderLabel> footer;  /// Optional labels to display on the bottom border
};

/// @struct Style
/// @brief Describes the visual appearance of a widget.
///
/// Includes colors, text decoration, borders, alignment, and padding.
/// Widgets typically have a default Style and a focused Style.
struct Style {
  Color fg = {255, 255, 255}; ///< Foreground (text) color
  Color bg = {0, 0, 0};       ///< Background color

  TextDecoration text; ///< Text decoration (bold, underline, etc.)

  Border border; ///< Border configuration

  Align innerAlignX =
      Align::Start; ///< Horizontal alignment of content within widget
  Align innerAlignY =
      Align::Start; ///< Vertical alignment of content within widget

  PaddingSpec
      padding; ///< Padding inside the widget, between border and content.
};

} // namespace vextr::core