#pragma once
#include <Vextr/core/Widget.hpp>
#include <functional>
#include <vector>
#include <memory>

namespace vextr::widgets {

class Navigator : public core::Widget {
public:
    // callbacks
    std::function<void(std::shared_ptr<Widget>)> onPush;
    std::function<void(std::shared_ptr<Widget>)> onPop;
    std::function<void(std::shared_ptr<Widget>)> onReplace;

    void push(std::shared_ptr<Widget> screen);
    void pop();
    void replace(std::shared_ptr<Widget> screen);

    std::shared_ptr<Widget> current() const;
    bool canPop() const { return stack.size() > 1; }
    int  depth()  const { return (int)stack.size(); }

    core::Size measure(int availW, int availH) override;
    void layout(int x, int y, int width, int height) override;
    void render(backend::Buffer& buf) override;
    bool onEvent(const core::Event& e) override;

    std::vector<std::shared_ptr<Widget>> getChildren() const override {
        if (stack.empty()) return {};
        return { stack.back() };
    }

private:
    std::vector<std::shared_ptr<Widget>> stack;
};

} // namespace vextr::widgets