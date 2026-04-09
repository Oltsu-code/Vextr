#include <Vextr/App.hpp>
#include <../include/Vextr/widgets/Container.hpp>
#include <../include/Vextr/layout/StackLayout.hpp>
#include <../include/Vextr/layout/GridLayout.hpp>
#include <Vextr/widgets/Label.hpp>

using namespace vextr::core;

int main() {
    auto root = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Horizontal)
    );
    root->setStyle(Style{.bg = {20, 20, 20}});

    // left sidebar
    auto left = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Vertical, 1)
    );
    left->setStyle(Style{.bg = {40, 40, 80}});

    auto leftLabel1 = std::make_shared<vextr::widgets::Label>("Item 1");
    leftLabel1->setStyle(Style{.fg = {255, 255, 255}, .bg = {60, 60, 120}});

    auto leftLabel2 = std::make_shared<vextr::widgets::Label>("Item 2");
    leftLabel2->setStyle(Style{.fg = {255, 255, 255}, .bg = {80, 40, 100}});

    auto leftLabel3 = std::make_shared<vextr::widgets::Label>("Item 3");
    leftLabel3->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 80, 100}});

    left->addChild(leftLabel1, {.ratio = 1.0f, .fixedH = 0.1f, .alignH = Align::Center, .alignV = Align::Center});
    left->addChild(leftLabel2, {.ratio = 1.0f});
    left->addChild(leftLabel3, {.ratio = 1.0f});

    // center
    auto center = std::make_shared<Container>(
        std::make_shared<GridLayout>(2, 2, 1, 1)
    );
    center->setStyle(Style{.bg = {20, 60, 20}});

    auto g1 = std::make_shared<vextr::widgets::Label>("Grid 0,0");
    g1->setStyle(Style{.fg = {255, 255, 100}, .bg = {40, 100, 40}});
    auto g2 = std::make_shared<vextr::widgets::Label>("Grid 1,0");
    g2->setStyle(Style{.fg = {255, 255, 100}, .bg = {60, 120, 60}});
    auto g3 = std::make_shared<vextr::widgets::Label>("Grid 0,1");
    g3->setStyle(Style{.fg = {255, 255, 100}, .bg = {40, 80, 80}});
    auto g4 = std::make_shared<vextr::widgets::Label>("Grid 1,1");
    g4->setStyle(Style{.fg = {255, 255, 100}, .bg = {60, 60, 100}});

    center->addChild(g1, {.col = 0, .row = 0});
    center->addChild(g2, {.col = 1, .row = 0});
    center->addChild(g3, {.col = 0, .row = 1});
    center->addChild(g4, {.col = 1, .row = 1});

    // right
    auto right = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Vertical, 0)
    );
    right->setStyle(Style{.bg = {80, 30, 30}});

    auto rightTop = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Horizontal, 1)
    );
    rightTop->setStyle(Style{.bg = {100, 40, 40}});

    auto rtLabel1 = std::make_shared<vextr::widgets::Label>("A");
    rtLabel1->setStyle(Style{.fg = {255, 200, 200}, .bg = {120, 50, 50}});
    auto rtLabel2 = std::make_shared<vextr::widgets::Label>("B");
    rtLabel2->setStyle(Style{.fg = {255, 200, 200}, .bg = {140, 60, 40}, .expand = false});

    rightTop->addChild(rtLabel1, {.ratio = 1.0f});
    rightTop->addChild(rtLabel2, {.ratio = 1.0f});

    auto rightBottom = std::make_shared<vextr::widgets::Label>("Bottom");
    rightBottom->setStyle(Style{.fg = {255, 150, 150}, .bg = {60, 20, 20}});

    right->addChild(rightTop,    {.ratio = 1.0f});
    right->addChild(rightBottom, {.ratio = 2.0f});

    root->addChild(left,   {.ratio = 1.0f});
    root->addChild(center, {.ratio = 2.0f});
    root->addChild(right,  {.ratio = 1.0f});

    vextr::App app;
    app.setRoot(root);
    app.run();

    return 0;
}