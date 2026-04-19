#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/utils/Input.hpp>
#include <Vextr/widgets/TextArea.hpp>
#include <gtest/gtest.h>

namespace vextr::widgets {

TEST(TextAreaTest, StartsEmptyAndFocusable) {
  TextArea area;
  EXPECT_TRUE(area.isFocusable());
  EXPECT_EQ(area.getText(), "");
}

TEST(TextAreaTest, HandlesTypingAndBackspace) {
  TextArea area;

  core::Event typeA{core::EventType::Key};
  typeA.key = 'a';
  core::Event typeB{core::EventType::Key};
  typeB.key = 'b';
  core::Event backspace{core::EventType::Key};
  backspace.key = utils::Key::Backspace;

  EXPECT_TRUE(area.onEvent(typeA));
  EXPECT_TRUE(area.onEvent(typeB));
  EXPECT_EQ(area.getText(), "ab");

  EXPECT_TRUE(area.onEvent(backspace));
  EXPECT_EQ(area.getText(), "a");
}

TEST(TextAreaTest, SupportsMultilineInsertionAndVerticalMovement) {
  TextArea area;
  area.setText("ab\ncd");

  core::Event up{core::EventType::Key};
  up.key = utils::Key::Up;
  core::Event enter{core::EventType::Key};
  enter.key = utils::Key::Enter;

  EXPECT_TRUE(area.onEvent(up));
  EXPECT_TRUE(area.onEvent(enter));
  EXPECT_EQ(area.getText(), "ab\n\ncd");
}

TEST(TextAreaTest, DrawsPlaceholderWhenEmpty) {
  TextArea area("type here");
  area.layout(0, 0, 9, 2);

  backend::Buffer buffer(9, 2);
  area.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "t");
  EXPECT_EQ(buffer.get(4, 0).ch, " ");
}

TEST(TextAreaTest, WrapModeRendersWrappedRows) {
  TextArea area;
  area.setFlowMode(TextFlowMode::Wrap);
  area.setText("hello world");
  area.layout(0, 0, 5, 2);

  backend::Buffer buffer(5, 2);
  area.render(buffer);

  EXPECT_EQ(buffer.get(0, 0).ch, "h");
  EXPECT_EQ(buffer.get(4, 0).ch, "o");
  EXPECT_EQ(buffer.get(0, 1).ch, "w");
  EXPECT_EQ(buffer.get(4, 1).ch, "d");
}

TEST(TextAreaTest, ScrollModeRendersHorizontalScrollbar) {
  TextArea area;
  area.setText("abcdef");
  area.layout(0, 0, 4, 2);

  backend::Buffer buffer(4, 2);
  area.render(buffer);

  EXPECT_EQ(buffer.get(0, 1).ch, "█");
  EXPECT_EQ(buffer.get(1, 1).ch, "█");
  EXPECT_EQ(buffer.get(2, 1).ch, "─");
  EXPECT_EQ(buffer.get(3, 1).ch, "─");
}

TEST(TextAreaTest, HorizontalScrollbarThumbMovesWhenScrolled) {
  TextArea area;
  area.setText("abcdef");
  area.layout(0, 0, 4, 2);

  core::Event left{core::EventType::Key};
  left.key = utils::Key::Left;
  EXPECT_TRUE(area.onEvent(left));

  backend::Buffer buffer(4, 2);
  area.render(buffer);

  EXPECT_EQ(buffer.get(0, 1).ch, "─");
  EXPECT_EQ(buffer.get(1, 1).ch, "─");
  EXPECT_EQ(buffer.get(2, 1).ch, "█");
  EXPECT_EQ(buffer.get(3, 1).ch, "█");
}

TEST(TextAreaTest, RendersVerticalScrollbarWhenRowsOverflow) {
  TextArea area;
  area.setText("a\nb\nc\nd");
  area.layout(0, 0, 4, 2);

  backend::Buffer buffer(4, 2);
  area.render(buffer);

  EXPECT_EQ(buffer.get(3, 0).ch, "█");
  EXPECT_EQ(buffer.get(3, 1).ch, "│");
}

TEST(TextAreaTest, RendersCornerWhenBothScrollbarsAreVisible) {
  TextArea area;
  area.setText("123456\na\nb\nc");
  area.layout(0, 0, 4, 3);

  backend::Buffer buffer(4, 3);
  area.render(buffer);

  EXPECT_EQ(buffer.get(3, 2).ch, "┼");
}

} // namespace vextr::widgets
