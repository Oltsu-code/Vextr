#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/FocusManager.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/ScrollView.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>

namespace vextr::widgets {

namespace {

class PatternWidget : public core::Widget {
public:
  PatternWidget(int width, int height) : size{width, height} {}

  core::Size measure(int, int) override { return size; }

protected:
  void drawContent(backend::Buffer &buf, core::Rect inner) override {
    for (int y = inner.y; y < inner.y + inner.height; ++y) {
      for (int x = inner.x; x < inner.x + inner.width; ++x) {
        backend::Cell cell;
        int localRow = y - getRect().y;
        cell.ch = std::string(1, static_cast<char>('0' + (localRow % 10)));
        buf.set(x, y, cell);
      }
    }
  }

private:
  core::Size size;
};

} // namespace

TEST(ScrollViewTest, StoresAndExposesContent) {
  auto content = std::make_shared<PatternWidget>(5, 4);
  ScrollView view(content);

  EXPECT_EQ(view.getContent(), content);
  EXPECT_TRUE(view.getChildren().empty());
  EXPECT_EQ(view.getFocusChildren().size(), 1u);
  EXPECT_EQ(view.getFocusChildren().front(), content);
}

TEST(ScrollViewTest, FocusTraversalReachesFocusableContent) {
  auto root = std::make_shared<Container>();
  auto content = std::make_shared<Container>();
  auto button = std::make_shared<Button>("Inside ScrollView");
  content->addChild(button, {.fixedH = core::cells(1)});

  auto scroll = std::make_shared<ScrollView>(content);
  root->addChild(scroll, {.ratio = 1.0f});

  core::FocusManager fm;
  fm.focusNext(root);
  EXPECT_EQ(fm.focused(), scroll);

  fm.focusNext(root);
  EXPECT_EQ(fm.focused(), button);
}

TEST(ScrollViewTest, LayoutClampsAndOffsetsChild) {
  auto content = std::make_shared<PatternWidget>(5, 4);
  ScrollView view(content);
  view.setContentSize({5, 4});

  view.layout(0, 0, 3, 2);
  EXPECT_EQ(content->getRect().x, 0);
  EXPECT_EQ(content->getRect().y, 0);
  EXPECT_EQ(content->getRect().width, 5);
  EXPECT_EQ(content->getRect().height, 4);

  view.setScroll(1, 1);
  EXPECT_EQ(content->getRect().x, -1);
  EXPECT_EQ(content->getRect().y, -1);
}

TEST(ScrollViewTest, RenderClipsToViewport) {
  auto content = std::make_shared<PatternWidget>(5, 4);
  ScrollView view(content);
  view.setContentSize({5, 4});
  view.layout(0, 0, 3, 2);
  view.setScroll(0, 1);

  backend::Buffer buffer(3, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "1");
  EXPECT_EQ(buffer.get(1, 0).ch, "1");
  EXPECT_EQ(buffer.get(2, 0).ch, "█");
  EXPECT_EQ(buffer.get(0, 1).ch, "2");
  EXPECT_EQ(buffer.get(1, 1).ch, "2");
  EXPECT_EQ(buffer.get(2, 1).ch, "│");
}

TEST(ScrollViewTest, EventScrollingMovesViewportAndClamps) {
  auto content = std::make_shared<PatternWidget>(5, 4);
  ScrollView view(content);
  view.setContentSize({5, 4});
  view.layout(0, 0, 3, 2);

  core::Event down{};
  down.type = core::EventType::Key;
  down.key = utils::Key::Down;

  EXPECT_TRUE(view.onEvent(down));
  EXPECT_EQ(view.scrollY(), 1);
  EXPECT_EQ(view.scrollX(), 0);

  EXPECT_TRUE(view.onEvent(down));
  EXPECT_EQ(view.scrollY(), 2);
  EXPECT_TRUE(view.onEvent(down));
  EXPECT_EQ(view.scrollY(), 2);

  core::Event pageUp{};
  pageUp.type = core::EventType::Key;
  pageUp.key = utils::Key::PageUp;
  EXPECT_TRUE(view.onEvent(pageUp));
  EXPECT_EQ(view.scrollY(), 0);

  core::Event end{};
  end.type = core::EventType::Key;
  end.key = utils::Key::End;
  EXPECT_TRUE(view.onEvent(end));
  EXPECT_EQ(view.scrollX(), 3);
  EXPECT_EQ(view.scrollY(), 2);

  core::Event home{};
  home.type = core::EventType::Key;
  home.key = utils::Key::Home;
  EXPECT_TRUE(view.onEvent(home));
  EXPECT_EQ(view.scrollX(), 0);
  EXPECT_EQ(view.scrollY(), 0);
}

TEST(ScrollViewTest, RenderShowsVerticalScrollIndicators) {
  auto content = std::make_shared<PatternWidget>(3, 4);
  ScrollView view(content);
  view.setContentSize({3, 4});
  view.layout(0, 0, 3, 2);
  view.setScroll(0, 1);

  backend::Buffer buffer(3, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(2, 0).ch, "█");
  EXPECT_EQ(buffer.get(2, 1).ch, "│");
}

} // namespace vextr::widgets
