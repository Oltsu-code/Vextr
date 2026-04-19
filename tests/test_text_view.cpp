#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/TextView.hpp>
#include <gtest/gtest.h>

namespace vextr::widgets {

TEST(TextViewTest, StoresTextFromConstructorAndSetter) {
  TextView view("line one");
  EXPECT_EQ(view.getText(), "line one");
  view.setText("line two");
  EXPECT_EQ(view.getText(), "line two");
}

TEST(TextViewTest, IsFocusableOnlyWhenContentOverflows) {
  TextView view("fit");
  view.layout(0, 0, 5, 1);
  EXPECT_FALSE(view.isFocusable());

  view.setText("averylongline");
  EXPECT_TRUE(view.isFocusable());
}

TEST(TextViewTest, MeasureUsesLongestLineAndLineCount) {
  TextView view("short\nlonger line\nmid");
  core::Size size = view.measure(100, 100);
  EXPECT_EQ(size.height, 3);
  EXPECT_GE(size.width, 11);
}

TEST(TextViewTest, WrapModeUsesAvailableWidth) {
  TextView view("hello world");
  view.setFlowMode(TextFlowMode::Wrap);

  core::Size size = view.measure(5, 100);
  EXPECT_EQ(size.height, 2);
  EXPECT_LE(size.width, 5);
}

TEST(TextViewTest, RenderDrawsVisibleLinesWithScroll) {
  TextView view("aaa\nbbb\nccc");
  view.layout(0, 0, 4, 2);
  view.setScrollY(1);

  backend::Buffer buffer(4, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "b");
  EXPECT_EQ(buffer.get(1, 0).ch, "b");
  EXPECT_EQ(buffer.get(0, 1).ch, "c");
}

TEST(TextViewTest, ScrollModeRespondsToKeyboardScrolling) {
  TextView view("abcdefghij\nsecond\nthird");
  view.layout(0, 0, 4, 2);

  core::Event down{core::EventType::Key};
  down.key = utils::Key::Down;
  core::Event right{core::EventType::Key};
  right.key = utils::Key::Right;
  core::Event pageDown{core::EventType::Key};
  pageDown.key = utils::Key::PageDown;
  core::Event end{core::EventType::Key};
  end.key = utils::Key::End;
  core::Event home{core::EventType::Key};
  home.key = utils::Key::Home;

  EXPECT_TRUE(view.onEvent(down));
  EXPECT_EQ(view.scrollY(), 1);

  EXPECT_TRUE(view.onEvent(right));
  EXPECT_EQ(view.scrollX(), 1);

  EXPECT_TRUE(view.onEvent(pageDown));
  EXPECT_EQ(view.scrollY(), 2);

  EXPECT_TRUE(view.onEvent(end));
  EXPECT_EQ(view.scrollX(), 7);
  EXPECT_EQ(view.scrollY(), 2);

  EXPECT_TRUE(view.onEvent(home));
  EXPECT_EQ(view.scrollX(), 0);
  EXPECT_EQ(view.scrollY(), 0);
}

TEST(TextViewTest, WrapModeIgnoresKeyboardScrolling) {
  TextView view("one\ntwo");
  view.setFlowMode(TextFlowMode::Wrap);
  view.layout(0, 0, 4, 2);

  core::Event down{core::EventType::Key};
  down.key = utils::Key::Down;

  EXPECT_FALSE(view.onEvent(down));
  EXPECT_EQ(view.scrollY(), 0);
}

TEST(TextViewTest, WrapModeRendersWrappedRows) {
  TextView view("hello world");
  view.setFlowMode(TextFlowMode::Wrap);
  view.layout(0, 0, 5, 2);

  backend::Buffer buffer(5, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "h");
  EXPECT_EQ(buffer.get(4, 0).ch, "o");
  EXPECT_EQ(buffer.get(0, 1).ch, "w");
  EXPECT_EQ(buffer.get(4, 1).ch, "d");
}

TEST(TextViewTest, ScrollModeRendersHorizontallyClippedContent) {
  TextView view("abcdef");
  view.layout(0, 0, 3, 1);
  view.setScrollX(2);

  backend::Buffer buffer(3, 1);
  view.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "c");
  EXPECT_EQ(buffer.get(1, 0).ch, "d");
  EXPECT_EQ(buffer.get(2, 0).ch, "e");
}

TEST(TextViewTest, RendersVerticalScrollIndicators) {
  TextView view("row1\nrow2\nrow3\nrow4");
  view.layout(0, 0, 5, 2);
  view.setScrollY(1);

  backend::Buffer buffer(5, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(4, 0).ch, "█");
  EXPECT_EQ(buffer.get(4, 1).ch, "│");
}

TEST(TextViewTest, RendersHorizontalScrollbarInScrollMode) {
  TextView view("abcdefgh");
  view.layout(0, 0, 4, 2);

  backend::Buffer buffer(4, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(0, 1).ch, "█");
  EXPECT_EQ(buffer.get(1, 1).ch, "█");
  EXPECT_EQ(buffer.get(2, 1).ch, "─");
  EXPECT_EQ(buffer.get(3, 1).ch, "─");
}

TEST(TextViewTest, WrapModeReservesScrollbarColumn) {
  TextView view("abcd efgh ijkl");
  view.setFlowMode(TextFlowMode::Wrap);
  view.layout(0, 0, 5, 2);

  backend::Buffer buffer(5, 2);
  view.render(buffer);

  EXPECT_EQ(buffer.get(3, 0).ch, "d");
  EXPECT_EQ(buffer.get(4, 0).ch, "█");
  EXPECT_EQ(buffer.get(3, 1).ch, "h");
  EXPECT_EQ(buffer.get(4, 1).ch, "│");
}

} // namespace vextr::widgets
