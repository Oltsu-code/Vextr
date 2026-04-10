#include <Vextr/utils/Unicode.hpp>
#include <gtest/gtest.h>

namespace vextr::utils::unicode {

class UnicodeTest : public ::testing::Test {
protected:
  // Common ASCII codepoints
  uint32_t ascii_a = 97;
  uint32_t ascii_A = 65;

  // Common wide characters
  uint32_t cjk_char = 0x4E00; // Chinese character
};

TEST_F(UnicodeTest, AsciiCharacterWidth) {
  // ASCII characters should have width 1
  int width = displayWidth(ascii_a);
  EXPECT_EQ(width, 1);
}

TEST_F(UnicodeTest, AsciiCapitalWidth) {
  // ASCII uppercase should also have width 1
  int width = displayWidth(ascii_A);
  EXPECT_EQ(width, 1);
}

TEST_F(UnicodeTest, SimpleAsciiString) {
  std::string ascii = "Hello";
  int width = stringWidth(ascii);
  EXPECT_EQ(width, 5);
}

TEST_F(UnicodeTest, EmptyString) {
  std::string empty = "";
  int width = stringWidth(empty);
  EXPECT_EQ(width, 0);
}

TEST_F(UnicodeTest, SingleCharacterString) {
  std::string single = "A";
  int width = stringWidth(single);
  EXPECT_EQ(width, 1);
}

TEST_F(UnicodeTest, StringWithNumbers) {
  std::string numbers = "12345";
  int width = stringWidth(numbers);
  EXPECT_EQ(width, 5);
}

TEST_F(UnicodeTest, StringWithSpaces) {
  std::string spaces = "Hello World";
  int width = stringWidth(spaces);
  EXPECT_EQ(width, 11); // "Hello World" is 11 characters
}

TEST_F(UnicodeTest, StringWithSpecialCharacters) {
  std::string special = "!@#$%";
  int width = stringWidth(special);
  EXPECT_EQ(width, 5);
}

TEST_F(UnicodeTest, StringWithTabs) {
  std::string tabs =
      "A\tB"; // Tab has width 1 in UTF-8 but may display differently
  int width = stringWidth(tabs);
  EXPECT_GT(width, 0); // Should have some width
}

TEST_F(UnicodeTest, LongAsciiString) {
  std::string longStr(100, 'a');
  int width = stringWidth(longStr);
  EXPECT_EQ(width, 100);
}

TEST_F(UnicodeTest, NumericString) {
  std::string numbers = "0123456789";
  int width = stringWidth(numbers);
  EXPECT_EQ(width, 10);
}

} // namespace vextr::utils::unicode
