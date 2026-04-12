#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Input.hpp>
#include <Vextr/widgets/Label.hpp>

#include "Vextr/utils/Debug.hpp"

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

int main() {
  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {15, 15, 25}});

  auto title = std::make_shared<Label>("Input demo - Tab to focus 💀💀💀💀💀");
  title->setStyle(Style{.fg = {200, 200, 255}, .bg = {25, 25, 50}});

  auto nameInput = std::make_shared<Input>("Enter your name...");
  nameInput->setStyle(Style{.fg = {255, 255, 255}, .bg = {30, 30, 50}});
  nameInput->setFocusedStyle(Style{.fg = {255, 255, 255}, .bg = {50, 50, 90}});

  auto searchInput = std::make_shared<Input>("Search...");
  searchInput->setStyle(Style{.fg = {255, 255, 255}, .bg = {30, 50, 30}});
  searchInput->setFocusedStyle(
      Style{.fg = {255, 255, 255}, .bg = {50, 90, 50}});

  auto status = std::make_shared<Label>("Type something...");
  status->setStyle(Style{.fg = {150, 150, 150}, .bg = {20, 20, 30}});

  nameInput->setOnChange(
      [&](const std::string &t) { status->setText("Name: " + t); });
  nameInput->setOnSubmit(
      [&](const std::string &t) { status->setText("Submitted: " + t); });
  searchInput->setOnChange(
      [&](const std::string &t) { status->setText("Searching: " + t); });

  auto submitBtn = std::make_shared<Button>("Submit");
  submitBtn->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 80, 40}});
  submitBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {80, 160, 80}, .text = {.bold = true}});
  submitBtn->setOnClick([&]() {
    status->setText("Submitted: " + nameInput->getText());
    VEXTR_LOG(nameInput->getText());
  });

  root->addChild(title, {.fixedH = pct(8)});
  root->addChild(nameInput, {.fixedH = pct(8),
                             .alignV = Align::Center,
                             .margin = MarginSpec::xy(cells(2), cells(0))});
  root->addChild(searchInput, {.fixedH = pct(8),
                               .alignV = Align::Center,
                               .margin = MarginSpec::xy(cells(2), cells(0))});
  root->addChild(
      submitBtn,
      {.fixedH = pct(8), .alignH = Align::Center, .alignV = Align::Center});
  root->addChild(status, {.fixedH = pct(8)});

  vextr::App app;
  app.setRoot(root);
  app.run();

  return 0;
}