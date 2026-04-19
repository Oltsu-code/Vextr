#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/utils/Debug.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/TextArea.hpp>
#include <Vextr/widgets/TextView.hpp>

using namespace vextr::core;
using namespace vextr::layout;
using namespace vextr::widgets;

int main() {
  vextr::App app;

  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {12, 14, 22}, .padding = PaddingSpec{1, 1, 1, 1}});

  auto title = std::make_shared<Label>(
      "Text widgets demo - TextView (read-only) + TextArea (editable)");
  title->setStyle(Style{.fg = {180, 220, 255}, .bg = {12, 14, 22}});

  auto textViewTitle = std::make_shared<Label>("TextView");
  textViewTitle->setStyle(Style{.fg = {130, 180, 240}, .bg = {12, 14, 22}});

  auto textView = std::make_shared<TextView>(
      "This is a read-only TextView.\n"
      "Use it for logs, help text, or descriptions.\n"
      "- It supports multiline content\n"
      "- It can be styled like other widgets\n"
      "\n"
      "Tab to move focus into the TextArea below.");
  textView->setStyle(Style{
      .fg = {230, 235, 245},
      .bg = {20, 24, 35},
      .border = {.style = BorderStyle::Single, .color = {90, 120, 170}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });
  textView->setFocusedStyle(Style{
      .fg = {255, 255, 255},
      .bg = {28, 34, 52},
      .border = {.style = BorderStyle::Double, .color = {140, 180, 240}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });
  textView->setFlowMode(TextFlowMode::Wrap);

  auto textViewMode = std::make_shared<Label>("TextView mode: Wrap");
  textViewMode->setStyle(Style{.fg = {160, 190, 220}, .bg = {12, 14, 22}});

  auto toggleTextViewMode =
      std::make_shared<Button>("Toggle TextView wrap/scroll");
  toggleTextViewMode->setStyle(
      Style{.fg = {220, 230, 255}, .bg = {40, 50, 70}});
  toggleTextViewMode->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {70, 90, 120}, .text = {.bold = true}});

  auto textAreaTitle = std::make_shared<Label>("TextArea");
  textAreaTitle->setStyle(Style{.fg = {130, 220, 170}, .bg = {12, 14, 22}});

  auto textArea = std::make_shared<TextArea>("Start typing here...");
  textArea->setText("You can edit this text.\n"
                    "Try Enter, Backspace, Delete,\n"
                    "and arrow keys.");
  textArea->setStyle(Style{
      .fg = {245, 245, 245},
      .bg = {22, 30, 24},
      .border = {.style = BorderStyle::Single, .color = {90, 170, 120}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });
  textArea->setFocusedStyle(Style{
      .fg = {255, 255, 255},
      .bg = {28, 40, 30},
      .border = {.style = BorderStyle::Double, .color = {140, 220, 170}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });

  auto textAreaMode = std::make_shared<Label>("TextArea mode: Scroll");
  textAreaMode->setStyle(Style{.fg = {160, 220, 190}, .bg = {12, 14, 22}});

  auto toggleTextAreaMode =
      std::make_shared<Button>("Toggle TextArea wrap/scroll");
  toggleTextAreaMode->setStyle(
      Style{.fg = {220, 255, 230}, .bg = {40, 70, 50}});
  toggleTextAreaMode->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {70, 120, 90}, .text = {.bold = true}});

  auto status =
      std::make_shared<Label>("Tab to focus TextArea. Edit text freely.");
  status->setStyle(Style{.fg = {160, 180, 200}, .bg = {12, 14, 22}});
  textArea->setOnChange([&](const std::string &t) {
    status->setText("TextArea length: " + std::to_string(t.size()));
  });

  toggleTextViewMode->setOnClick([&]() {
    auto mode = textView->getFlowMode();
    mode = (mode == TextFlowMode::Wrap) ? TextFlowMode::Scroll
                                        : TextFlowMode::Wrap;
    textView->setFlowMode(mode);
    textViewMode->setText(std::string("TextView mode: ") +
                          (mode == TextFlowMode::Wrap ? "Wrap" : "Scroll"));
  });

  toggleTextAreaMode->setOnClick([&]() {
    auto mode = textArea->getFlowMode();
    mode = (mode == TextFlowMode::Wrap) ? TextFlowMode::Scroll
                                        : TextFlowMode::Wrap;
    textArea->setFlowMode(mode);
    textAreaMode->setText(std::string("TextArea mode: ") +
                          (mode == TextFlowMode::Wrap ? "Wrap" : "Scroll"));
  });

  auto saveBtn = std::make_shared<Button>("Save");
  saveBtn->setStyle(Style{.fg = {255, 255, 255}, .bg = {40, 80, 40}});
  saveBtn->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {70, 140, 70}, .text = {.bold = true}});
  saveBtn->setOnClick([&]() {
    status->setText("Saved! Text length: " +
                    std::to_string(textArea->getText().size()));
    VEXTR_LOG("Saved text: " + textArea->getText());
  });

  auto quitBtn = std::make_shared<Button>("Quit");
  quitBtn->setStyle(Style{
      .fg = {255, 220, 220}, .bg = {70, 30, 30}, .innerAlignX = Align::Center});
  quitBtn->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {120, 50, 50}, .text = {.bold = true}});
  quitBtn->setOnClick([&]() { app.quit(); });

  auto btnContainer = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 1));
  btnContainer->setStyle(
      Style{.bg = {12, 14, 22}, .innerAlignX = Align::Center});
  btnContainer->addChild(saveBtn, {.ratio = 1.0f, .alignH = Align::Start});
  btnContainer->addChild(quitBtn, {.ratio = 1.0f, .alignH = Align::End});

  root->addChild(title, {.fixedH = cells(1)});
  root->addChild(textViewTitle, {.fixedH = cells(1)});
  root->addChild(textViewMode, {.fixedH = cells(1)});
  root->addChild(toggleTextViewMode, {.fixedH = cells(1)});
  root->addChild(textView, {.ratio = 1.0f});
  root->addChild(textAreaTitle, {.fixedH = cells(1)});
  root->addChild(textAreaMode, {.fixedH = cells(1)});
  root->addChild(toggleTextAreaMode, {.fixedH = cells(1)});
  root->addChild(textArea, {.ratio = 1.0f});
  root->addChild(status, {.fixedH = cells(1)});
  root->addChild(btnContainer, {.fixedH = cells(1)});

  app.setRoot(root);
  app.run();
  return 0;
}
