#include <Vextr/utils/InputParser.hpp>
#include <gtest/gtest.h>

namespace vextr::utils {

class InputParserTest : public ::testing::Test {
protected:
  InputParser parser;
};

TEST_F(InputParserTest, SimpleKeyEvent) {
  parser.feed('a');
  EXPECT_TRUE(parser.hasEvent());
  auto event = parser.nextEvent();
  EXPECT_EQ(event.key, 'a');
  EXPECT_FALSE(event.shift);
  EXPECT_FALSE(event.ctrl);
  EXPECT_FALSE(event.alt);
}

TEST_F(InputParserTest, MultipleKeyEvents) {
  parser.feed('a');
  parser.feed('b');
  parser.feed('c');

  EXPECT_TRUE(parser.hasEvent());
  auto e1 = parser.nextEvent();
  EXPECT_EQ(e1.key, 'a');

  EXPECT_TRUE(parser.hasEvent());
  auto e2 = parser.nextEvent();
  EXPECT_EQ(e2.key, 'b');

  EXPECT_TRUE(parser.hasEvent());
  auto e3 = parser.nextEvent();
  EXPECT_EQ(e3.key, 'c');

  EXPECT_FALSE(parser.hasEvent());
}

TEST_F(InputParserTest, EnterKeyFromNewline) {
  parser.feed(10); // newline
  EXPECT_TRUE(parser.hasEvent());
  auto event = parser.nextEvent();
  EXPECT_EQ(event.key, 10); // should be newline/enter
}

TEST_F(InputParserTest, EscapeKeyPending) {
  parser.feed(27); // ESC
  EXPECT_TRUE(parser.pendingEscape());
  EXPECT_FALSE(parser.hasEvent()); // escape is pending, not yet emitted
}

TEST_F(InputParserTest, EscapeKeyFlush) {
  parser.feed(27); // ESC
  EXPECT_TRUE(parser.pendingEscape());
  parser.flushEscape();
  EXPECT_FALSE(parser.pendingEscape());
  EXPECT_TRUE(parser.hasEvent());
  auto event = parser.nextEvent();
  EXPECT_EQ(event.key, 27); // Escape key
}

TEST_F(InputParserTest, NoEventsInitially) { EXPECT_FALSE(parser.hasEvent()); }

TEST_F(InputParserTest, EmptyAfterConsumingAllEvents) {
  parser.feed('x');
  parser.nextEvent();
  EXPECT_FALSE(parser.hasEvent());
}

TEST_F(InputParserTest, NumericalKeyEvents) {
  parser.feed('1');
  parser.feed('2');
  parser.feed('3');

  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(parser.nextEvent().key, '1');
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(parser.nextEvent().key, '2');
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(parser.nextEvent().key, '3');
}

TEST_F(InputParserTest, SpecialCharacterKeys) {
  parser.feed(' ');  // space
  parser.feed('\t'); // tab

  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(parser.nextEvent().key, ' ');
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(parser.nextEvent().key, '\t');
}

} // namespace vextr::utils
