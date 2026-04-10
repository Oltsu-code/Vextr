#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <string>

namespace vextr::widgets {

class Button : public core::Widget {
public:
  explicit Button(const std::string &label = "") : label(label) {}

  void setLabel(const std::string &l) { label = l; }
  const std::string &getLabel() const { return label; }

  void setOnClick(std::function<void()> cb) { onClick = std::move(cb); }

  bool isFocusable() const override { return true; }

  core::Size measure(int availW, int availH) override;
  void render(backend::Buffer &buf) override;
  bool onEvent(const core::Event &e) override;

private:
  std::string label;
  std::function<void()> onClick;
};

} // namespace vextr::widgets