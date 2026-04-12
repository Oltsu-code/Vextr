#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

int main() {
  vextr::App app;

  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {15, 15, 25}});

  auto title =
      std::make_shared<Label>("Button demo - Tab to focus, Enter to click");
  title->setStyle(Style{.fg = {150, 150, 200}, .bg = {25, 25, 40}});

  auto status = std::make_shared<Label>("nothing clicked yet");
  status->setStyle(Style{.fg = {100, 200, 100}, .bg = {15, 30, 15}});

  auto btn1 = std::make_shared<Button>("Click me");
  btn1->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 40, 80}});
  btn1->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {80, 80, 160}, .text = {.bold = true}});
  btn1->setOnClick([&]() { status->setText("Button 1 clicked!"); });

  auto btn2 = std::make_shared<Button>("Another button");
  btn2->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 80, 40}});
  btn2->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {80, 160, 80}, .text = {.bold = true}});
  btn2->setOnClick([&]() { status->setText("Button 2 clicked!"); });

  auto btn3 = std::make_shared<Button>("Quit");
  btn3->setStyle(Style{.fg = {255, 200, 200}, .bg = {80, 30, 30}});
  btn3->setFocusedStyle(
      Style{.fg = {255, 100, 100}, .bg = {160, 60, 60}, .text = {.bold = true}});
  btn3->setOnClick([&]() { app.quit(); });

  auto row = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 2));
  row->setStyle(Style{.bg = {15, 15, 25}});
  row->addChild(
      btn1, {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});
  row->addChild(
      btn2, {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});
  row->addChild(
      btn3, {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});

  root->addChild(title, {.fixedH = pct(10)});
  root->addChild(row, {.ratio = 1.0f});
  root->addChild(status, {.fixedH = pct(10)});

  app.setRoot(root);
  app.run();

  return 0;
}