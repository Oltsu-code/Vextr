#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Align.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/core/Geometry.hpp>
#include <Vextr/core/Style.hpp>
#include <memory>
#include <vector>

namespace vextr::core {

/// @class Widget
/// @brief The base class for all UI components.
///
/// Widget is the foundation of the UI system. All visual components (buttons,
/// labels, containers, etc.) inherit from this class. Widgets form a tree
/// hierarchy where each widget can have a parent and children.
///
/// **Key responsibilities:**
/// - Measure: Calculate how much space is needed
/// - Layout: Position and size the widget and its children
/// - Render: Draw to the screen buffer
/// - Events: Handle user input
/// - Focus: Manage keyboard focus
///
/// **Subclassing:**
/// To create a custom widget, override these virtual methods:
/// - measure(): Return your size requirements
/// - layout(): Position yourself and children (call parent class if needed)
/// - drawContent(): Render your content (not render()!)
/// - onEvent(): Handle input events
/// - isFocusable(): Return true if your widget can receive focus
///
/// @note When overriding render(), be very careful - the framework's border and
/// background rendering relies on the default implementation. Use drawContent()
/// instead.
class Widget : public std::enable_shared_from_this<Widget> {
public:
  virtual ~Widget() = default;

  // ---- Layout lifecycle ----

  /// @brief Measures how much space this widget needs.
  ///
  /// Called during the measurement pass to determine widget size requirements.
  /// Should return the preferred size based on available space.
  ///
  /// @param availW Maximum available width (may be ignored by widget)
  /// @param availH Maximum available height (may be ignored by widget)
  /// @return The Size this widget wants, or needs at minimum
  virtual Size measure(int availW, int availH);

  /// @brief Positions and sizes this widget and its children.
  ///
  /// Called after measurement to actually place the widget at a specific
  /// location. If your widget has children, you must call their layout()
  /// methods here.
  ///
  /// @param x The X coordinate for this widget
  /// @param y The Y coordinate for this widget
  /// @param width The width allocated to this widget
  /// @param height The height allocated to this widget
  virtual void layout(int x, int y, int width, int height);

  /// @brief Renders this widget to the buffer.
  ///
  /// The default implementation handles drawing borders and background.
  /// **WARNING**: Overriding this will break border and background rendering!
  /// Use drawContent() instead to draw your widget's content.
  ///
  /// @param buf The buffer to draw to
  virtual void render(backend::Buffer &buf);

  // ---- Input and focus ----

  /// @brief Handles an input event.
  ///
  /// Return true if you handled the event, false to pass it to the parent.
  /// The framework may stop event propagation if true is returned.
  ///
  /// @param event The input event
  /// @return True if this widget handled the event
  virtual bool onEvent(const Event &event) { return false; }

  /// @brief Returns whether this widget can receive keyboard focus.
  ///
  /// Widgets like Button and Input return true; containers return false.
  ///
  /// @return True if this widget can be focused, false otherwise
  virtual bool isFocusable() const { return false; }

  /// @brief Called when this widget gains keyboard focus.
  virtual void onFocus() {}

  /// @brief Called when this widget loses keyboard focus.
  virtual void onBlur() {}

  /// @brief Checks if this widget currently has keyboard focus.
  /// @return True if this widget is focused
  bool focused() const;

  // ---- Styling ----

  /// @brief Sets the widget's normal style.
  /// @param s The style to apply
  void setStyle(const Style &s) { style = s; }

  /// @brief Sets the widget's style when it has focus.
  /// @param s The focused style to apply
  void setFocusedStyle(const Style &s) { focusedStyle = s; }

  /// @brief Gets the widget's normal style.
  /// @return The current style
  const Style &getStyle() const { return style; }

  /// @brief Gets the widget's focused style.
  /// @return The focused style
  const Style &getFocusedStyle() const { return focusedStyle; }

  /// @brief Gets this widget's bounding rectangle.
  /// @return The position and size of this widget
  const Rect &getRect() const { return rect; }

  // ---- Widget tree ----

  /// @brief Returns this widget's children.
  /// @return Vector of child widgets (empty if no children)
  virtual std::vector<std::shared_ptr<Widget>> getChildren() const {
    return {};
  }

  /// @brief Weak pointer to this widget's parent.
  /// Use weak_ptr to avoid circular references.
  std::weak_ptr<Widget> parent;

protected:
  /// @brief Draws this widget's content.
  ///
  /// Called by render() after background and borders are drawn.
  /// This is where you should draw your widget's actual content.
  /// The inner rectangle accounts for borders and padding.
  ///
  /// @param buf The buffer to draw to
  /// @param inner The inner rectangle (excluding border/padding)
  virtual void drawContent(backend::Buffer &buf, Rect inner) {}

  /// @brief Gets the currently active style (normal or focused).
  /// @return The normal style if not focused, focused style if focused
  const Style &activeStyle() const;

  /// @brief Gets the inner rectangle, accounting for border and padding.
  /// @return The inner rectangle (content area)
  Rect innerRect() const;

  /// @brief The normal (unfocused) style
  Style style;

  /// @brief The style applied when this widget has focus
  Style focusedStyle;

  /// @brief This widget's bounding rectangle (set during layout)
  Rect rect;

  int justifyX(int innerX, int innerW, int contentW, Align align) const;
  int justifyY(int innerY, int innerH, int contentH, Align align) const;

  Rect alignContentRect(Rect inner, int contentW, int contentH, Align ax,
                        Align ay) const;

private:
  void drawBackground(backend::Buffer &buf);
  void drawBorder(backend::Buffer &buf);
};

} // namespace vextr::core