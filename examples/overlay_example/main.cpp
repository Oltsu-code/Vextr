#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Dialog.hpp>
#include <Vextr/widgets/Dropdown.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/Modal.hpp>

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

int main() {
  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  Style rootStyle;
  rootStyle.bg = {15, 15, 25};
  root->setStyle(rootStyle);

  // title
  auto title = std::make_shared<Label>(
      "Overlay Demo — Tab to focus, Enter to activate, Q to quit");
  Style titleStyle;
  titleStyle.fg = {180, 180, 255};
  titleStyle.bg = {25, 25, 45};
  titleStyle.innerAlignX = Align::Center;
  title->setStyle(titleStyle);
  root->addChild(title, {.fixedH = cells(1)});

  // status label updated by interactions
  auto status = std::make_shared<Label>("Nothing selected yet.");
  Style statusStyle;
  statusStyle.fg = {150, 200, 150};
  statusStyle.bg = {15, 15, 25};
  statusStyle.innerAlignX = Align::Center;
  status->setStyle(statusStyle);

  // ── Dropdown ───────────────────────────────────────────────────────────────
  auto dropSection = std::make_shared<Label>("Dropdown:");
  Style secStyle;
  secStyle.fg = {180, 180, 255};
  secStyle.bg = {15, 15, 25};
  dropSection->setStyle(secStyle);
  root->addChild(dropSection, {.fixedH = cells(1)});

  auto dropdown = std::make_shared<Dropdown>(std::vector<Dropdown::Option>{
      {"Apple", "apple"},
      {"Banana", "banana"},
      {"Cherry", "cherry"},
      {"Durian", "durian"},
      {"Elderberry", "elderberry"},
      {"Fig", "fig"},
      {"Grape", "grape"},
      {"Honeydew", "honeydew"},
      {"Jackfruit", "jackfruit"},
      {"Kiwi", "kiwi"},
      {"Lemon", "lemon"},
      {"Mango", "mango"},
      {"Nectarine", "nectarine"},
      {"Orange", "orange"},
      {"Papaya", "papaya"},
  });
  Style dropStyle;
  dropStyle.fg = {255, 255, 255};
  dropStyle.bg = {35, 35, 60};
  dropStyle.border.style = BorderStyle::Rounded;
  dropStyle.border.color = {100, 100, 200};
  dropStyle.padding = PaddingSpec::xy(1, 0);
  dropdown->setStyle(dropStyle);

  Style dropFocused = dropStyle;
  dropFocused.border.color = {180, 180, 255};
  dropFocused.fg = {255, 255, 100};
  dropdown->setFocusedStyle(dropFocused);

  dropdown->setPopupStyle(dropStyle);
  dropdown->setPopupFocusedStyle(dropFocused);

  dropdown->setOnChange([status](int idx, const std::string &val) {
    status->setText("Dropdown selected: " + val);
  });
  root->addChild(dropdown,
                 {.fixedW = cells(50), .fixedH = cells(3), .sizeW = pct(40)});

  // ── Dialog button ──────────────────────────────────────────────────────────
  auto dialogSection = std::make_shared<Label>("Dialog:");
  dialogSection->setStyle(secStyle);
  root->addChild(dialogSection, {.fixedH = cells(1)});

  auto dialogBtn = std::make_shared<Button>("Show Confirm Dialog");
  Style btnStyle;
  btnStyle.fg = {255, 255, 255};
  btnStyle.bg = {40, 40, 80};
  btnStyle.border.style = BorderStyle::Single;
  btnStyle.border.color = {80, 80, 160};
  btnStyle.innerAlignX = Align::Center;
  btnStyle.innerAlignY = Align::Center;
  dialogBtn->setStyle(btnStyle);

  Style btnFocused = btnStyle;
  btnFocused.fg = {255, 255, 100};
  btnFocused.text.bold = true;
  btnFocused.border.style = BorderStyle::Double;
  btnFocused.border.color = {180, 180, 255};
  dialogBtn->setFocusedStyle(btnFocused);

  dialogBtn->setOnClick([status]() {
    showDialog(
        "Confirm Action", "Are you sure you want to do this?",
        {
            {"Yes", [status]() { status->setText("Dialog: confirmed!"); },
             true},
            {"No", [status]() { status->setText("Dialog: cancelled."); }},
        });
  });
  root->addChild(dialogBtn, {
                                .fixedH = cells(3),
                                .sizeW = pct(40),
                                .alignH = Align::Center,
                            });

  // ── Custom modal button ────────────────────────────────────────────────────
  auto modalSection = std::make_shared<Label>("Custom Modal:");
  modalSection->setStyle(secStyle);
  root->addChild(modalSection, {.fixedH = cells(1)});

  auto modalBtn = std::make_shared<Button>("Show Custom Modal");
  modalBtn->setStyle(btnStyle);
  modalBtn->setFocusedStyle(btnFocused);
  modalBtn->setOnClick([status, btnStyle, btnFocused]() {
    auto content = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Vertical, 1));
    Style cs;
    cs.bg = {25, 25, 45};
    cs.border.style = BorderStyle::Rounded;
    cs.border.color = {100, 200, 100};
    cs.border.title = {
        {"Custom Modal", Align::Center, {.bold = true}, {150, 255, 150}}};
    cs.padding = PaddingSpec::all(1);
    content->setStyle(cs);

    auto msg = std::make_shared<Label>(
        "This is a custom modal.\nPress Escape to close.");
    Style ms;
    ms.fg = {220, 255, 220};
    ms.bg = {25, 25, 45};
    ms.innerAlignX = Align::Center;
    msg->setStyle(ms);
    content->addChild(msg, {.ratio = 1.0f});

    auto closeBtn = std::make_shared<Button>("Close");
    closeBtn->setStyle(btnStyle);
    closeBtn->setFocusedStyle(btnFocused);
    closeBtn->setOnClick([status]() {
      status->setText("Custom modal closed.");
      closeModal();
    });
    content->addChild(closeBtn, {
                                    .fixedH = cells(3),
                                    .sizeW = pct(30),
                                    .alignH = Align::Center,
                                });

    showModal(content,
              [status]() { status->setText("Custom modal dismissed."); });
  });
  root->addChild(
      modalBtn,
      {.fixedH = cells(3), .sizeW = pct(40), .alignH = Align::Center});

  // status at bottom
  root->addChild(status, {.fixedH = cells(1)});

  vextr::App app;
  app.setRoot(root);
  app.run();
  return 0;
}