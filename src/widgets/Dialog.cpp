#include <Vextr/core/Context.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Dialog.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/Modal.hpp>

namespace vextr::widgets {

void showDialog(const std::string &title, const std::string &message,
                std::vector<DialogButton> buttons) {

  // build content container
  auto box = std::make_shared<Container>(
      std::make_shared<layout::StackLayout>(layout::Axis::Vertical, 1));

  core::Style boxStyle;
  boxStyle.bg = {25, 25, 45};
  boxStyle.border.style = core::BorderStyle::Rounded;
  boxStyle.border.color = {100, 100, 220};
  boxStyle.border.title = {
      {title, core::Align::Center, {.bold = true}, {180, 180, 255}}};
  boxStyle.padding = core::PaddingSpec::xy(2, 1);
  box->setStyle(boxStyle);

  // message label
  auto msg = std::make_shared<Label>(message);
  core::Style msgStyle;
  msgStyle.bg = {25, 25, 45};
  msgStyle.fg = {220, 220, 255};
  msgStyle.innerAlignX = core::Align::Center;
  msg->setStyle(msgStyle);
  box->addChild(msg, {.ratio = 1.0f});

  // button row
  auto btnRow = std::make_shared<Container>(
      std::make_shared<layout::StackLayout>(layout::Axis::Horizontal, 1));
  core::Style rowStyle;
  rowStyle.bg = {25, 25, 45};
  btnRow->setStyle(rowStyle);

  for (auto &db : buttons) {
    auto btn = std::make_shared<Button>(db.label);

    core::Style bs;
    bs.fg = {255, 255, 255};
    bs.bg = db.isDefault ? core::Color{40, 80, 40} : core::Color{50, 50, 70};
    bs.border.style = core::BorderStyle::Single;
    bs.border.color =
        db.isDefault ? core::Color{80, 180, 80} : core::Color{80, 80, 120};
    bs.innerAlignX = core::Align::Center;
    bs.innerAlignY = core::Align::Center;
    btn->setStyle(bs);

    core::Style bfs = bs;
    bfs.fg = {255, 255, 100};
    bfs.text.bold = true;
    bfs.border.style = core::BorderStyle::Double;
    bfs.border.color = {255, 255, 100};
    btn->setFocusedStyle(bfs);

    auto cb = db.onClick;
    btn->setOnClick([cb]() {
      if (cb)
        cb();
      core::Context::get().overlayManager.pop();
    });

    btnRow->addChild(btn, {.ratio = 1.0f,
                           .sizeH = core::cells(3),
                           .alignH = core::Align::Center,
                           .alignV = core::Align::Center});
  }

  box->addChild(btnRow, {.fixedH = core::cells(5)});

  showModal(box, []() { core::Context::get().overlayManager.pop(); });
}

} // namespace vextr::widgets