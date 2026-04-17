#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace vextr::widgets {

/// @class Navigator
/// @brief A screen navigation widget that manages a stack of screens.
///
/// Navigator implements a stack-based navigation pattern, allowing you to push,
/// pop, and replace screens. It maintains a stack of widgets and always
/// displays the topmost one. This is useful for implementing multi-screen
/// applications with back button support.
///
/// **Example Usage:**
/// ```cpp
/// auto nav = std::make_shared<Navigator>();
/// auto home = std::make_shared<HomeScreen>();
/// nav->push(home);
///
/// // When user clicks a button to go to another screen:
/// auto details = std::make_shared<DetailsScreen>();
/// nav->push(details);
///
/// // When user clicks back:
/// nav->pop();
/// ```
class Navigator : public core::Widget {
public:
  /// @brief Callback fired when a screen is pushed onto the stack.
  /// @param screen The widget that was pushed.
  std::function<void(std::shared_ptr<Widget>)> onPush;

  /// @brief Callback fired when a screen is popped from the stack.
  /// @param screen The widget that was popped.
  std::function<void(std::shared_ptr<Widget>)> onPop;

  /// @brief Callback fired when a screen is replaced.
  /// @param screen The widget that replaced the previous one.
  std::function<void(std::shared_ptr<Widget>)> onReplace;

  /// @brief Pushes a widget onto the navigation stack.
  ///
  /// Makes the given widget the current active screen. The previous screen
  /// remains on the stack and can be returned to via pop().
  /// Triggers the onPush callback.
  ///
  /// @param screen The widget to push onto the stack. Must not be null.
  void push(std::shared_ptr<Widget> screen);

  /// @brief Pops the current widget from the stack.
  ///
  /// Removes the topmost widget from the stack and displays the one beneath it.
  /// Does nothing if the stack has only one widget (cannot pop the root).
  /// Triggers the onPop callback.
  void pop();

  /// @brief Replaces the current widget with a new one.
  ///
  /// Removes the topmost widget and pushes a new one in its place. This is
  /// useful for transitions that don't preserve history.
  /// Triggers the onReplace callback.
  ///
  /// @param screen The widget to replace the current one with. Must not be
  /// null.
  void replace(std::shared_ptr<Widget> screen);

  /// @brief Returns the currently active widget.
  ///
  /// @return A shared pointer to the topmost widget on the stack,
  ///         or nullptr if the stack is empty.
  std::shared_ptr<Widget> current() const;

  /// @brief Checks if the stack can be popped.
  ///
  /// Returns true if there is more than one widget on the stack.
  /// Always returns false if there is only one (root) widget.
  ///
  /// @return True if pop() would succeed, false otherwise.
  bool canPop() const { return stack.size() > 1; }

  /// @brief Returns the current depth of the navigation stack.
  ///
  /// @return The number of widgets currently on the stack.
  int depth() const { return (int)stack.size(); }

  core::Size measure(int availW, int availH) override;
  void layout(int x, int y, int width, int height) override;
  void render(backend::Buffer &buf) override;
  bool onEvent(const core::Event &e) override;

  std::vector<std::shared_ptr<Widget>> getChildren() const override {
    if (stack.empty())
      return {};
    return {stack.back()};
  }

private:
  std::vector<std::shared_ptr<Widget>> stack;
};

} // namespace vextr::widgets