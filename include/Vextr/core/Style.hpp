#pragma once
#include <Vextr/core/Align.hpp>
#include <cstdint>

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

/// @struct Border
/// @brief Describes the border styling for a widget.
struct Border {
  BorderStyle style = BorderStyle::None; ///< The border style
  Color color = {255, 255, 255};         ///< The border color
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

  int padding = 0; ///< Padding in cells on all sides
};

} // namespace vextr::core