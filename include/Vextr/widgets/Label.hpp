#pragma once
#include <Vextr/core/Widget.hpp>
#include <string>

namespace vextr::widgets {

class Label : public core::Widget {
public:
  Label() = default;
  explicit Label(const std::string &text) : text(text) {}

  void setText(const std::string &t) { text = t; }
  const std::string &getText() const { return text; }

  core::Size measure(int availW, int availH) override;

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override;

private:
  std::string text;
};

} // namespace vextr::widgets
