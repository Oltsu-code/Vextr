#pragma once
#include <Vextr/widgets/Container.hpp>
#include <functional>
#include <string>
#include <vector>

namespace vextr::widgets {

/// @struct DialogButton
/// @brief Describes a button in a dialog.
struct DialogButton {
  std::string label;
  std::function<void()> onClick;
  bool isDefault = false; ///< Highlighted as the primary action
};

/// @brief Shows a modal dialog with a title, message, and buttons.
///
/// The dialog closes automatically when any button is clicked.
/// Pressing Escape closes it if a cancel/close button exists.
///
/// @example
/// ```cpp
/// showDialog("Confirm", "Are you sure?", {
///   {"Yes", [&]() { doThing(); }, true},
///   {"No",  []()  {}             }
/// });
/// ```
void showDialog(const std::string &title, const std::string &message,
                std::vector<DialogButton> buttons);

} // namespace vextr::widgets