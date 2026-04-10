#include <Vextr/App.hpp>
#include <Vextr/layout/AbsoluteLayout.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/utils/Debug.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/Navigator.hpp>

using namespace vextr::core;
using namespace vextr::widgets;

int main() {
  auto nav = std::make_shared<Navigator>();

  nav->onPush = [](auto s) { VEXTR_LOG("pushed screen"); };
  nav->onPop = [](auto s) { VEXTR_LOG("popped screen"); };
  nav->onReplace = [](auto s) { VEXTR_LOG("replaced screen"); };

  // ── main screen ──────────────────────────────────────────────
  auto mainScreen = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  mainScreen->setStyle(Style{.bg = {15, 15, 25}});

  auto mainTitle = std::make_shared<Label>("Main Screen - Tab to focus");
  mainTitle->setStyle(Style{.fg = {200, 200, 255}, .bg = {25, 25, 50}});

  auto openBtn = std::make_shared<Button>("Open Settings");
  openBtn->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 40, 80}});
  openBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {80, 80, 160}, .bold = true});

  auto dialogBtn = std::make_shared<Button>("Open Dialog");
  dialogBtn->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 80, 40}});
  dialogBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {80, 160, 80}, .bold = true});

  // ── settings screen ──────────────────────────────────────────
  auto settingsScreen = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  settingsScreen->setStyle(Style{.bg = {25, 15, 35}});

  auto settingsTitle = std::make_shared<Label>("Settings Screen");
  settingsTitle->setStyle(Style{.fg = {200, 150, 255}, .bg = {40, 25, 60}});

  auto backBtn = std::make_shared<Button>("Go Back");
  backBtn->setStyle(Style{.fg = {255, 200, 255}, .bg = {60, 30, 80}});
  backBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {100, 60, 140}, .bold = true});
  backBtn->setOnClick([&]() { nav->pop(); });

  settingsScreen->addChild(settingsTitle, {.fixedH = pct(15)});
  settingsScreen->addChild(
      backBtn,
      {.fixedH = pct(10), .alignH = Align::Center, .alignV = Align::Center});

  // ── dialog overlay ───────────────────────────────────────────
  auto dialogOverlay =
      std::make_shared<Container>(std::make_shared<AbsoluteLayout>());
  // transparent bg - main screen shows through
  dialogOverlay->setStyle(Style{.bg = Color::clear()});

  auto dialogBox = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  dialogBox->setStyle(Style{.bg = {40, 40, 60}, .padding = 1});

  auto dialogTitle = std::make_shared<Label>("Are you sure?");
  dialogTitle->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 40, 60}});

  auto confirmBtn = std::make_shared<Button>("Yes");
  confirmBtn->setStyle(Style{.fg = {100, 255, 100}, .bg = {30, 60, 30}});
  confirmBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {60, 120, 60}, .bold = true});
  confirmBtn->setOnClick([&]() { nav->pop(); });

  auto cancelBtn = std::make_shared<Button>("Cancel");
  cancelBtn->setStyle(Style{.fg = {255, 100, 100}, .bg = {60, 30, 30}});
  cancelBtn->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {120, 60, 60}, .bold = true});
  cancelBtn->setOnClick([&]() { nav->pop(); });

  auto btnRow = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 2));
  btnRow->setStyle(Style{.bg = {40, 40, 60}});
  btnRow->addChild(
      confirmBtn,
      {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});
  btnRow->addChild(
      cancelBtn,
      {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});

  dialogBox->addChild(dialogTitle, {.fixedH = pct(40)});
  dialogBox->addChild(btnRow, {.ratio = 1.0f});

  dialogOverlay->addChild(dialogBox, {
                                         .fixedW = pct(40),
                                         .fixedH = pct(30),
                                         .absX = pct(30),
                                         .absY = pct(35),
                                     });

  // wire up main screen buttons
  openBtn->setOnClick([&]() { nav->push(settingsScreen); });
  dialogBtn->setOnClick([&]() { nav->push(dialogOverlay); });

  mainScreen->addChild(mainTitle, {.fixedH = pct(15)});
  mainScreen->addChild(
      openBtn,
      {.fixedH = pct(10), .alignH = Align::Center, .alignV = Align::Center});
  mainScreen->addChild(
      dialogBtn,
      {.fixedH = pct(10), .alignH = Align::Center, .alignV = Align::Center});

  nav->push(mainScreen);

  vextr::App app;
  app.setRoot(nav);
  app.run();

  return 0;
}