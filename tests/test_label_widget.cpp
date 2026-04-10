#include <Vextr/backend/Buffer.hpp>
#include <Vextr/widgets/Label.hpp>
#include <gtest/gtest.h>

namespace vextr::widgets {

class LabelTest : public ::testing::Test {
protected:
  Label label;
};

TEST_F(LabelTest, DefaultConstructor) { EXPECT_EQ(label.getText(), ""); }

TEST_F(LabelTest, ConstructorWithText) {
  Label lbl("Hello World");
  EXPECT_EQ(lbl.getText(), "Hello World");
}

TEST_F(LabelTest, SetAndGetText) {
  label.setText("Test Label");
  EXPECT_EQ(label.getText(), "Test Label");
}

TEST_F(LabelTest, SetTextMultipleTimes) {
  label.setText("First");
  EXPECT_EQ(label.getText(), "First");

  label.setText("Second");
  EXPECT_EQ(label.getText(), "Second");

  label.setText("Third");
  EXPECT_EQ(label.getText(), "Third");
}

TEST_F(LabelTest, EmptyText) {
  label.setText("");
  EXPECT_EQ(label.getText(), "");
}

TEST_F(LabelTest, MeasureEmptyText) {
  auto size = label.measure(100, 100);
  EXPECT_EQ(size.height, 1); // labels are always 1 line tall
}

TEST_F(LabelTest, MeasureSimpleText) {
  label.setText("Hello");
  auto size = label.measure(100, 100);
  EXPECT_EQ(size.height, 1);
  // Width should be the string width (at least 5 for "Hello")
  EXPECT_GE(size.width, 5);
}

TEST_F(LabelTest, MeasureRespectsBounds) {
  label.setText("This is a longer label");
  auto size = label.measure(50, 10);
  EXPECT_EQ(size.height, 1);
}

TEST_F(LabelTest, MultipleLabelsIndependent) {
  Label label1("Label One");
  Label label2("Label Two");

  EXPECT_EQ(label1.getText(), "Label One");
  EXPECT_EQ(label2.getText(), "Label Two");

  label1.setText("Modified");
  EXPECT_EQ(label1.getText(), "Modified");
  EXPECT_EQ(label2.getText(), "Label Two"); // unchanged
}

TEST_F(LabelTest, LongText) {
  std::string longText(200, 'a');
  label.setText(longText);
  EXPECT_EQ(label.getText(), longText);
}

TEST_F(LabelTest, SpecialCharacters) {
  label.setText("!@#$%^&*()");
  EXPECT_EQ(label.getText(), "!@#$%^&*()");
}

TEST_F(LabelTest, RenderWithValidBuffer) {
  label.setText("Test");
  backend::Buffer buffer(50, 10);
  // This should not throw
  EXPECT_NO_THROW(label.render(buffer));
}

} // namespace vextr::widgets
