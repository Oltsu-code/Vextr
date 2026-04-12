#include <Vextr/App.hpp>
#include <Vextr/layout/AbsoluteLayout.hpp>
#include <Vextr/layout/GridLayout.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

int main() {
  // root: horizontal stack
  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal));
  root->setStyle(Style{.bg = {15, 15, 15}});

  // ── LEFT: vertical stack, equal ratio children ──────────────────
  auto left = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  left->setStyle(Style{.bg = {30, 30, 60}, .padding = 1});

  auto l1 = std::make_shared<Label>("Stretch (default)");
  l1->setStyle(Style{.fg = {200, 200, 255}, .bg = {50, 50, 100}});

  auto l2 = std::make_shared<Label>("Center H+V");
  l2->setStyle(Style{.fg = {200, 200, 255}, .bg = {70, 50, 120}});

  auto l3 = std::make_shared<Label>("Start H, End V");
  l3->setStyle(Style{.fg = {200, 200, 255}, .bg = {50, 70, 120}});

  auto l4 = std::make_shared<Label>("Fixed 30% height");
  l4->setStyle(Style{.fg = {255, 255, 150}, .bg = {80, 80, 40}});

  left->addChild(l1, {.ratio = 1.0f});
  left->addChild(
      l2, {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});
  left->addChild(l3,
                 {.ratio = 1.0f, .alignH = Align::Start, .alignV = Align::End});
  left->addChild(l4, {.fixedH = pct(30)});

  // ── CENTER: 3x2 grid with spans and margins ──────────────────────
  auto center =
      std::make_shared<Container>(std::make_shared<GridLayout>(3, 2, 1, 1));
  center->setStyle(Style{.bg = {15, 40, 15}});

  auto g1 = std::make_shared<Label>("0,0 span2");
  g1->setStyle(Style{.fg = {150, 255, 150}, .bg = {30, 80, 30}});

  auto g2 = std::make_shared<Label>("2,0");
  g2->setStyle(Style{.fg = {150, 255, 150}, .bg = {20, 60, 60}});

  auto g3 = std::make_shared<Label>("0,1");
  g3->setStyle(Style{.fg = {150, 255, 150}, .bg = {20, 60, 40}});

  auto g4 = std::make_shared<Label>("1,1 center");
  g4->setStyle(Style{.fg = {255, 255, 100}, .bg = {60, 60, 20}});

  auto g5 = std::make_shared<Label>("2,1 margin");
  g5->setStyle(Style{.fg = {150, 255, 150}, .bg = {60, 20, 60}});

  center->addChild(g1, {.col = 0, .row = 0, .colSpan = 2});
  center->addChild(g2, {.col = 2, .row = 0});
  center->addChild(g3, {.col = 0, .row = 1});
  center->addChild(
      g4,
      {.alignH = Align::Center, .alignV = Align::Center, .col = 1, .row = 1});
  center->addChild(g5,
                   {.margin = MarginSpec::all(cells(1)), .col = 2, .row = 1});

  // ── RIGHT: nested stacks ─────────────────────────────────────────
  auto right = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 0));
  right->setStyle(Style{.bg = {40, 15, 15}});

  // top: horizontal with ratio 2:1
  auto rightTop = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 1));
  rightTop->setStyle(Style{.bg = {60, 20, 20}});

  auto rt1 = std::make_shared<Label>("ratio 2");
  rt1->setStyle(Style{.fg = {255, 180, 180}, .bg = {100, 40, 40}});
  auto rt2 = std::make_shared<Label>("ratio 1");
  rt2->setStyle(Style{.fg = {255, 180, 180}, .bg = {80, 30, 60}});

  rightTop->addChild(rt1, {.ratio = 2.0f, .alignV = Align::Center});
  rightTop->addChild(rt2, {.ratio = 1.0f, .alignV = Align::Center});

  // middle: absolute layout test
  auto rightMid =
      std::make_shared<Container>(std::make_shared<AbsoluteLayout>());
  rightMid->setStyle(Style{.bg = {20, 20, 50}});

  auto abs1 = std::make_shared<Label>("abs 10%,10%");
  abs1->setStyle(Style{.fg = {200, 200, 255}, .bg = {60, 60, 120}});
  auto abs2 = std::make_shared<Label>("abs 40%,50%");
  abs2->setStyle(Style{.fg = {255, 200, 100}, .bg = {80, 60, 20}});

  rightMid->addChild(abs1, {.absX = pct(10), .absY = pct(10)});
  rightMid->addChild(abs2, {.absX = pct(40), .absY = pct(50)});

  // bottom: fixed height label, end-aligned
  auto rightBot = std::make_shared<Label>("fixed 20% h, end");
  rightBot->setStyle(Style{.fg = {255, 150, 100}, .bg = {50, 20, 10}});

  right->addChild(rightTop, {.ratio = 1.0f});
  right->addChild(rightMid, {.ratio = 2.0f});
  right->addChild(
      rightBot,
      {.fixedH = pct(20), .alignH = Align::End, .alignV = Align::Center});

  // ── root children ────────────────────────────────────────────────
  root->addChild(left, {.ratio = 1.0f});
  root->addChild(center, {.ratio = 2.0f});
  root->addChild(right, {.ratio = 1.0f});

  vextr::App app;
  app.setRoot(root);
  app.run();

  return 0;
}