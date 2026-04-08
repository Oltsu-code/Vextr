#pragma once
#include "Vextr/core/Widget.hpp"
#include <string>

namespace vextr::widgets {

class Button : public core::Widget{
public:
    Button() = default;
    explicit Button(const std::string& text) : text(text) {}

    void setText(const std::string& t) { text = t; }
    const std::string& getText() const { return text; }

    core::Size measure(int availW, int availH) override;
    void render(backend::Buffer& buf) override;

    bool isFocusable() const override { return true; }
    bool onEvent(const core::Event& e) override;

private:
    std::string text;
};

} // vextr::widgets
