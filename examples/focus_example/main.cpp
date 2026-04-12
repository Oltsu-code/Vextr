#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Label.hpp>

using namespace vextr::core;
using namespace vextr::widgets;
using namespace vextr::layout;

// a focusable label - lights up when focused
class FocusableLabel : public Label {
public:
  explicit FocusableLabel(const std::string &text) : Label(text) {}

  bool isFocusable() const override { return true; }

  void onFocus() override {
    // swap to focused style handled by activeStyle()
  }

  bool onEvent(const Event &e) override {
    if (e.type == EventType::Key && e.key == vextr::utils::Key::Enter) {
      // enter pressed while focused

      if (getText().contains("[") && getText().contains("]"))
        setText(getText().substr(1, getText().size() - 2));
      else
        setText("[" + getText() + "]");

      return true;
    }
    return false;
  }

  void render(vextr::backend::Buffer &buf) override {
    // use activeStyle() instead of style directly
    vextr::backend::Cell base;
    base.ch = ' ';
    base.fg = activeStyle().fg;
    base.bg = activeStyle().bg;
    for (int y = rect.y; y < rect.y + rect.height; ++y)
      for (int x = rect.x; x < rect.x + rect.width; ++x)
        buf.set(x, y, base);

    int cx = rect.x;
    int maxX = rect.x + rect.width;
    for (unsigned char c : getText()) {
      if (cx >= maxX)
        break;
      vextr::backend::Cell cell;
      cell.ch = c;
      cell.fg = activeStyle().fg;
      cell.bg = activeStyle().bg;
      cell.bold = activeStyle().bold;
      cell.underline = activeStyle().underline;
      buf.set(cx, rect.y, cell);
      cx++;
    }
  }
};

int main() {
  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {15, 15, 25}});

  // header - not focusable
  auto header = std::make_shared<Label>(
      "Tab to move focus  Enter to select  Q to quit");
  header->setStyle(Style{.fg = {150, 150, 150}, .bg = {25, 25, 40}});

  // three focusable items
  auto item1 = std::make_shared<FocusableLabel>("  Option 1");
  item1->setStyle(Style{.fg = {200, 200, 255}, .bg = {30, 30, 60}});
  item1->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {60, 60, 120}, .bold = true});

  auto item2 = std::make_shared<FocusableLabel>("  Option 2");
  item2->setStyle(Style{.fg = {200, 200, 255}, .bg = {30, 30, 60}});
  item2->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {60, 60, 120}, .bold = true});

  auto item3 = std::make_shared<FocusableLabel>("  Option 3");
  item3->setStyle(Style{.fg = {200, 200, 255}, .bg = {30, 30, 60}});
  item3->setFocusedStyle(
      Style{.fg = {255, 255, 100}, .bg = {60, 60, 120}, .bold = true});

  // a non-focusable label at the bottom
  auto footer = std::make_shared<vextr::widgets::Label>("  Status: ready");
  footer->setStyle(Style{.fg = {100, 200, 100}, .bg = {15, 30, 15}});

  // horizontal row with two focusable items side by side
  auto row = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 1));
  row->setStyle(Style{.bg = {15, 15, 25}});

  auto left = std::make_shared<FocusableLabel>("  Left panel");
  left->setStyle(Style{.fg = {200, 180, 255}, .bg = {40, 20, 60}});
  left->setFocusedStyle(
      Style{.fg = {255, 220, 100}, .bg = {80, 40, 120}, .bold = true});

  auto right = std::make_shared<FocusableLabel>("  Right panel");
  right->setStyle(Style{.fg = {200, 180, 255}, .bg = {40, 20, 60}});
  right->setFocusedStyle(
      Style{.fg = {255, 220, 100}, .bg = {80, 40, 120}, .bold = true});

  row->addChild(left, {.ratio = 1.0f});
  row->addChild(right, {.ratio = 1.0f});

  root->addChild(header, {.fixedH = pct(8)});
  root->addChild(item1, {.fixedH = pct(10)});
  root->addChild(item2, {.fixedH = pct(10)});
  root->addChild(item3, {.fixedH = pct(10)});
  root->addChild(row, {.ratio = 1.0f});
  root->addChild(footer, {.fixedH = pct(8)});

  vextr::App app;
  app.setRoot(root);
  app.run();

  return 0;
}