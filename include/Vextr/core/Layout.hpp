#pragma once
#include <Vextr/core/Align.hpp>
#include <Vextr/core/Geometry.hpp>
#include <climits>
#include <memory>
#include <vector>

namespace vextr::core {

class Widget;

/// @struct ScalarSpec
/// @brief Represents a single dimension value that can be absolute or
/// percentage-based.
///
/// Used for sizing, margins, and positioning. Values can be:
/// - Absolute: measured in cells (characters)
/// - Percentage: measured as % of parent size
///
/// @example
/// ```cpp
/// ScalarSpec width = pct(50);        // 50% of parent width
/// ScalarSpec height = cells(10);      // 10 cells tall
/// ```
struct ScalarSpec {
  float value = 0.f; ///< The numeric value
  bool percent =
      false; ///< If true, value is a percentage; if false, it's in cells

  /// @brief Returns true if this spec was explicitly set.
  /// @return True if value > 0
  bool isSet() const { return value > 0.f; }

  /// @brief Resolves this spec to an absolute cell count.
  /// @param parentSize The parent size (used if this is a percentage)
  /// @return The resolved size in cells
  int resolve(int parentSize) const {
    if (!isSet())
      return 0;
    if (percent)
      return static_cast<int>(parentSize * value / 100.f);
    return static_cast<int>(value);
  }
};

/// @brief Helper to create a percentage-based ScalarSpec.
/// @param v The percentage value (0-100)
/// @return A ScalarSpec representing v% of parent size
inline ScalarSpec pct(float v) { return {v, true}; }

/// @brief Helper to create an absolute cell-based ScalarSpec.
/// @param v The size in cells
/// @return A ScalarSpec representing v cells
inline ScalarSpec cells(float v) { return {v, false}; }

/// @struct MarginSpec
/// @brief Specifies spacing around all four sides.
///
/// Can be configured per-side or using helper functions.
struct MarginSpec {
  ScalarSpec top, bottom, left, right; ///< Spacing on each side

  /// @brief Helper: Create equal margins on all sides.
  static MarginSpec all(ScalarSpec v) { return {v, v, v, v}; }

  /// @brief Helper: Create margins with separate X and Y values.
  static MarginSpec xy(ScalarSpec x, ScalarSpec y) { return {y, y, x, x}; }

  /// @brief Helper: Create zero margins.
  static MarginSpec none() { return {}; }

  /// @brief Resolves top margin.
  int resolvedTop(int h) const { return top.resolve(h); }

  /// @brief Resolves bottom margin.
  int resolvedBottom(int h) const { return bottom.resolve(h); }

  /// @brief Resolves left margin.
  int resolvedLeft(int w) const { return left.resolve(w); }

  /// @brief Resolves right margin.
  int resolvedRight(int w) const { return right.resolve(w); }
};

/// @struct LayoutSpec
/// @brief Specifies how a widget should be sized and positioned within its
/// layout slot.
///
/// This is passed to Container::addChild() to control layout behavior.
/// Different layout engines may use different fields.
///
/// **Common usage:**
/// ```cpp
/// // Widget takes 50% of available width
/// container->addChild(widget, LayoutSpec{.fixedW = pct(50)});
///
/// // Widget has equal weight with siblings (stretches)
/// container->addChild(widget, LayoutSpec{.ratio = 1.0f});
/// ```
struct LayoutSpec {
  // ---- Slot sizing ----

  /// Relative weight among siblings in stack layouts.
  /// Used when dividing remaining space among children.
  float ratio = 1.0f;

  /// Fixed slot width - overrides ratio in horizontal stacks
  ScalarSpec fixedW;

  /// Fixed slot height - overrides ratio in vertical stacks
  ScalarSpec fixedH;

  // ---- Widget sizing within slot ----

  /// If set, widget is this size inside its slot.
  /// If not set, widget fills slot according to alignH/alignV.
  ScalarSpec sizeW;
  ScalarSpec sizeH;

  // ---- Constraints ----
  ScalarSpec minW, minH; ///< Minimum widget size
  ScalarSpec maxW, maxH; ///< Maximum widget size

  // ---- Alignment ----
  /// Alignment of widget within slot.
  /// Only meaningful when sizeW/sizeH is set or widget is smaller than slot.
  Align alignH = Align::Stretch; ///< Horizontal alignment
  Align alignV = Align::Stretch; ///< Vertical alignment

  // ---- Spacing ----
  MarginSpec margin; ///< Spacing around widget

  // ---- Grid layout ----
  int col = 0;     ///< Column in grid (0-based)
  int row = 0;     ///< Row in grid (0-based)
  int colSpan = 1; ///< How many columns this widget spans
  int rowSpan = 1; ///< How many rows this widget spans

  // ---- Absolute layout ----
  ScalarSpec absX; ///< Absolute X position
  ScalarSpec absY; ///< Absolute Y position
};

/// @struct ChildSlot
/// @brief Internal structure pairing a widget with its layout spec.
/// @internal
struct ChildSlot {
  std::shared_ptr<Widget> widget; ///< The child widget
  LayoutSpec spec;                ///< Its layout specification
};

/// @class Layout
/// @brief Abstract base class for layout engines.
///
/// Implement this to create custom layouts. The apply() method is called
/// with a list of child widgets and their specs, and should position each
/// child by setting their rect field.
class Layout {
public:
  virtual ~Layout() = default;

  /// @brief Applies the layout algorithm to position children.
  ///
  /// This method should position each child by modifying its rect field.
  /// The framework calls measure() before layout(), so you can inspect
  /// each widget's measured size.
  ///
  /// @param children The child widgets with their layout specs
  /// @param inner The inner area available for children (accounting for
  /// borders/padding)
  virtual void apply(std::vector<ChildSlot> &children, Rect inner) = 0;
};

} // namespace vextr::core