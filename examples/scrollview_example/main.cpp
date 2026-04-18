#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/ScrollView.hpp>
#include <memory>
#include <string>

#include "Vextr/widgets/Button.hpp"
#include "Vextr/widgets/Dialog.hpp"

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

int main() {
  vextr::App app;

  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {12, 12, 18}, .padding = PaddingSpec{1,1,1,1}});

  auto title = std::make_shared<Label>("ScrollView demo - Arrow keys, PageUp/Down, Home/End");
  title->setStyle(Style{.fg = {180, 220, 255}, .bg = {12, 12, 18}});

  auto instructions = std::make_shared<Label>(
      "This example shows a long list inside a clipped viewport.");
  instructions->setStyle(Style{.fg = {180, 180, 180}, .bg = {12, 12, 18}});

  auto content = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 0));
  content->setStyle(Style{.bg = {20, 22, 30}, .padding = PaddingSpec{1,1,1,1}});

  for (int i = 1; i <= 24; ++i) {
    auto line = std::make_shared<Label>(
        "Item " + std::to_string(i) + " - keep scrolling to see more");
    line->setStyle(Style{.fg = {240, 240, 240}, .bg = {20, 22, 30}});
    content->addChild(line, {.fixedH = cells(1)});
  }

  auto btn = std::make_shared<Button>("Click me");
  btn->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 40, 80}});
  btn->setFocusedStyle(Style{.fg = {255, 255, 255}, .bg = {60, 60, 120}});
  btn->setOnClick([]() {
    showDialog("Button clicked", "You clicked the button!", {
      {"OK", []() {}}
    });
  });
  content->addChild(btn, {.fixedH = cells(1)});

  auto scrollView = std::make_shared<ScrollView>(content);
  scrollView->setStyle(Style{
      .fg = {255, 255, 255},
      .bg = {24, 28, 38},
      .border = {.style = BorderStyle::Single, .color = {90, 120, 180}},
      .padding = PaddingSpec{1,1,1,1},
  });
  scrollView->setFocusedStyle(Style{
      .fg = {255, 255, 255},
      .bg = {34, 40, 56},
      .border = {.style = BorderStyle::Double, .color = {180, 220, 255}},
      .padding = PaddingSpec{1,1,1,1},
  });
  scrollView->setContentSize({0, 28});

  auto footer = std::make_shared<Label>("Tip: use End to jump to the bottom.");
  footer->setStyle(Style{.fg = {150, 200, 150}, .bg = {12, 12, 18}});

  root->addChild(title, {.fixedH = cells(1)});
  root->addChild(instructions, {.fixedH = cells(1)});
  root->addChild(scrollView, {.ratio = 1.0f});
  root->addChild(footer, {.fixedH = cells(1)});

  app.setRoot(root);
  app.run();
  return 0;
}



