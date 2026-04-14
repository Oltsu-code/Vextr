#pragma once
#include <Vextr/core/Widget.hpp>
#include <Vextr/widgets/Container.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

/// @class Modal
/// @brief A full-screen dimmed overlay with a centered content box.
///
/// Modal renders a semi-transparent backdrop and centers its content
/// widget on screen. Press Escape to dismiss unless dismissable = false.
class Modal : public core::Widget {
public:
  explicit Modal(std::shared_ptr<Widget> content,
                 std::function<void()> onClose = {}, bool dismissable = true);

  core::Size measure(int availW, int availH) override;
  void layout(int x, int y, int width, int height) override;
  bool onEvent(const core::Event &e) override;
  bool isFocusable() const override { return false; }
  void handleResize(int termWidth, int termHeight);

  std::vector<std::shared_ptr<Widget>> getChildren() const override {
    return {content};
  }

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::shared_ptr<core::Widget> content;
  std::function<void()> onClose;
  bool dismissable;

  // content box size as fraction of terminal
  float contentW = 0.5f;
  float contentH = 0.4f;
};

/// @brief Helper to push a modal onto the overlay stack.
void showModal(std::shared_ptr<core::Widget> content,
               std::function<void()> onClose = {}, bool dismissable = true);

/// @brief Helper to close the topmost overlay (modal or otherwise).
void closeModal();

} // namespace vextr::widgets