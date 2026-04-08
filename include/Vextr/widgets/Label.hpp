#pragma once
#include <Vextr/core/Widget.hpp>
#include <string>

namespace vextr::widgets {

class Label : public core::Widget {
public:
    Label() = default;
    explicit Label(const std::string& text) : text(text) {}

    void setText(const std::string& t) { text = t; }
    const std::string& getText() const { return text; }

    core::Size measure(int availW, int availH) override;
    void render(backend::Buffer& buf) override;

    bool isFocusable() const override { return true; }

private:
    std::string text;
};

} // vextr::widgets
