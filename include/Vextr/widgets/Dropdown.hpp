#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <string>
#include <vector>

namespace vextr::widgets {

/// @class Dropdown
/// @brief A focusable widget that shows a popup list of options when opened.
class Dropdown : public core::Widget {
public:
  struct Option {
    std::string label;
    std::string value;
  };

  explicit Dropdown(std::vector<Option> options = {});

  /// @brief Sets the list of options to display.
  /// @param opts The new list of options
  void setOptions(std::vector<Option> opts) { options = std::move(opts); }

  /// @brief Sets the callback invoked when an option is selected.
  /// @param cb The function to call when an option is selected
  void setOnChange(std::function<void(int, const std::string &)> cb) {
    onChange = std::move(cb);
  }

  /// @brief Sets the style for the dropdown's popup list.
  /// @param s The style to apply to the popup
  void setPopupStyle(const core::Style &s) { popupStyle = s; }

  /// @brief Sets the style for the dropdown's popup list when focused.
  /// @param s The style to apply to the popup when focused
  void setPopupFocusedStyle(const core::Style &s) { popupFocusedStyle = s; }

  int selectedIndex() const { return selected; }
  const std::string &selectedValue() const { return options[selected].value; }

  /// @brief Gets the current width of the dropdown.
  /// @return the width in columns
  int getWidth() const { return rect.width; }

  bool isFocusable() const override { return true; }
  core::Size measure(int availW, int availH) override;
  bool onEvent(const core::Event &e) override;

  void layout(int x, int y, int width, int height) override;

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::vector<Option> options;
  int selected = 0;
  bool open = false;
  core::Style popupStyle;
  core::Style popupFocusedStyle;

  void openPopup();
  void closePopup();

  std::function<void(int, const std::string &)> onChange;
};

/// @class DropdownPopup
/// @brief The floating list that appears when a Dropdown is opened.
/// Managed by OverlayManager, not added to the widget tree directly.
class DropdownPopup : public core::Widget {
public:
  /// @brief Constructs a dropdown popup.
  /// @param options the list of options to display
  /// @param initialSelected the initially selected option index
  /// @param onSelect callback when an option is selected
  /// @param onClose callback when the popup is closed
  /// @param parentDropdown weak reference to the parent dropdown widget
  DropdownPopup(const std::vector<Dropdown::Option> &options,
                int initialSelected, std::function<void(int)> onSelect,
                std::function<void()> onClose,
                std::weak_ptr<Dropdown> parentDropdown);

  core::Size measure(int availW, int availH) override;
  bool onEvent(const core::Event &e) override;
  bool isFocusable() const override { return true; }

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  const std::vector<Dropdown::Option> &options;
  int hovered;
  int initialSelected;
  int scrollOffset = 0;
  std::weak_ptr<Dropdown> parentDropdown;
  std::function<void(int)> onSelect;
  std::function<void()> onClose;
};

} // namespace vextr::widgets