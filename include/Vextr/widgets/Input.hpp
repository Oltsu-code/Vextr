#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

class Input : public core::Widget {
public:
  explicit Input(const std::string &placeholder = "")
      : placeholder(placeholder) {}

  void setText(const std::string &t) {
    text = t;
    cursor = (int)t.size();
  }
  const std::string &getText() const { return text; }

  void setPlaceholder(const std::string &p) { placeholder = p; }
  void setOnChange(std::function<void(const std::string &)> cb) {
    onChange = std::move(cb);
  }
  void setOnSubmit(std::function<void(const std::string &)> cb) {
    onSubmit = std::move(cb);
  }

  bool isFocusable() const override { return true; }

  core::Size measure(int availW, int availH) override;
  void render(backend::Buffer &buf) override;
  bool onEvent(const core::Event &e) override;

private:
  std::string text;
  std::string placeholder;
  int cursor = 0; // cursor position in text
  int scroll = 0; // horizontal scroll offset

  std::function<void(const std::string &)> onChange;
  std::function<void(const std::string &)> onSubmit;

  void insertChar(char c);
  void deleteBack();
  void deleteForward();
  void moveCursor(int delta);
  void updateScroll();
};

} // namespace vextr::widgets