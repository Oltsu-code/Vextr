#include <Vextr/App.hpp>
#include <Vextr/layout/GridLayout.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Input.hpp>
#include <Vextr/widgets/Label.hpp>

using namespace vextr::core;
using namespace vextr::widgets;

int main() {
  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {15, 15, 25}});

  // top row — different border styles
  auto topRow = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 1));
  topRow->setStyle(Style{.bg = {15, 15, 25}});

  auto makeBorderBox = [](const std::string &label, BorderStyle bs,
                          Color borderColor) {
    auto box = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Vertical));
    Style s;
    s.bg = {25, 25, 45};
    s.border.style = bs;
    s.border.color = borderColor;
    s.padding = 1;
    box->setStyle(s);

    auto lbl = std::make_shared<Label>(label);
    lbl->setStyle(Style{.fg = {200, 200, 255}, .bg = {25, 25, 45}});
    box->addChild(
        lbl, {.ratio = 1.0f, .alignH = Align::Center, .alignV = Align::Center});
    return box;
  };

  topRow->addChild(
      makeBorderBox("Single", BorderStyle::Single, {100, 100, 255}),
      {.ratio = 1.0f});
  topRow->addChild(
      makeBorderBox("Double", BorderStyle::Double, {100, 255, 100}),
      {.ratio = 1.0f});
  topRow->addChild(
      makeBorderBox("Rounded", BorderStyle::Rounded, {255, 100, 100}),
      {.ratio = 1.0f});
  topRow->addChild(makeBorderBox("Thick", BorderStyle::Thick, {255, 255, 100}),
                   {.ratio = 1.0f});
  topRow->addChild(
      makeBorderBox("Dashed", BorderStyle::Dashed, {100, 255, 255}),
      {.ratio = 1.0f});

  // middle — input with border
  auto inputBox = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  Style inputBoxStyle;
  inputBoxStyle.bg = {20, 20, 40};
  inputBoxStyle.border.style = BorderStyle::Rounded;
  inputBoxStyle.border.color = {80, 80, 200};
  inputBoxStyle.padding = 1;
  inputBox->setStyle(inputBoxStyle);

  auto inputLabel = std::make_shared<Label>("Type something:");
  inputLabel->setStyle(Style{.fg = {150, 150, 200}, .bg = {20, 20, 40}});

  auto input = std::make_shared<Input>("Enter text...");
  input->setStyle(Style{.fg = {255, 255, 255},
                        .bg = {30, 30, 60},
                        .contentAlignX = Align::Center,
                        .contentAlignY = Align::Center});
  input->setFocusedStyle(Style{.fg = {255, 255, 255}, .bg = {50, 50, 100}});

  inputBox->addChild(inputLabel, {.fixedH = cells(1)});
  inputBox->addChild(input, {.fixedH = cells(1)});

  // bottom — buttons with borders
  auto btnRow = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 2));
  btnRow->setStyle(Style{.bg = {15, 15, 25}});

  auto makeBtn = [](const std::string &label, Color bg, Color borderColor) {
    auto btn = std::make_shared<Button>(label);

    Style s;
    s.fg = {255, 255, 255};
    s.bg = bg;

    s.border.style = BorderStyle::Single;
    s.border.color = borderColor;

    s.contentAlignX = Align::Center;
    s.contentAlignY = Align::Center;

    btn->setStyle(s);

    Style fs;
    fs.fg = {255, 255, 100};
    fs.bg = bg;
    fs.bold = true;
    fs.underline = true;

    fs.border.style = BorderStyle::Double;
    fs.border.color = {255, 255, 100};

    fs.contentAlignX = Align::Center;
    fs.contentAlignY = Align::Center;

    btn->setFocusedStyle(fs);

    return btn;
  };

  auto btn1 = makeBtn("OK", {30, 60, 30}, {50, 150, 50});
  auto btn2 = makeBtn("Cancel", {60, 30, 30}, {150, 50, 50});
  auto btn3 = makeBtn("Help", {30, 30, 60}, {50, 50, 150});

  btnRow->addChild(btn1, {.ratio = 1.0f,
                          .sizeW = pct(50.0f),
                          .sizeH = pct(40.0f),
                          .alignH = Align::Center,
                          .alignV = Align::Center});
  btnRow->addChild(btn2, {.ratio = 1.0f,
                          .sizeW = pct(50.0f),
                          .sizeH = pct(40.0f),
                          .alignH = Align::Center,
                          .alignV = Align::Center});
  btnRow->addChild(btn3, {.ratio = 1.0f,
                          .sizeW = pct(50.0f),
                          .sizeH = pct(40.0f),
                          .alignH = Align::Center,
                          .alignV = Align::Center});

  root->addChild(topRow, {.ratio = 1.0f});
  root->addChild(inputBox, {.fixedH = pct(20)});
  root->addChild(btnRow, {.fixedH = pct(50)});

  vextr::App app;
  app.setRoot(root);
  app.run();

  return 0;
}