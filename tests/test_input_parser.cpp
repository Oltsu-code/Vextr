#include <Vextr/utils/InputParser.hpp>
#include <gtest/gtest.h>

namespace vextr::utils {

class InputParserTest : public ::testing::Test {
protected:
  InputParser parser;

  // helper to avoid repeating get_if everywhere
  core::events::KeyEvent getKey(core::events::Event e) {
    auto *k = std::get_if<core::events::KeyEvent>(&e);
    EXPECT_NE(k, nullptr) << "Event is not a KeyEvent";
    return k ? *k : core::events::KeyEvent{};
  }
};

TEST_F(InputParserTest, SimpleKeyEvent) {
  parser.feed('a');
  EXPECT_TRUE(parser.hasEvent());
  auto k = getKey(parser.nextEvent());
  EXPECT_EQ(k.key, 'a');
  EXPECT_FALSE(k.modifiers.shift);
  EXPECT_FALSE(k.modifiers.ctrl);
  EXPECT_FALSE(k.modifiers.alt);
}

TEST_F(InputParserTest, MultipleKeyEvents) {
  parser.feed('a');
  parser.feed('b');
  parser.feed('c');

  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(getKey(parser.nextEvent()).key, 'a');
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(getKey(parser.nextEvent()).key, 'b');
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(getKey(parser.nextEvent()).key, 'c');
  EXPECT_FALSE(parser.hasEvent());
}

TEST_F(InputParserTest, EnterKeyFromNewline) {
  parser.feed(10);
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(getKey(parser.nextEvent()).key, 10);
}

TEST_F(InputParserTest, EscapeKeyPending) {
  parser.feed(27);
  EXPECT_TRUE(parser.pendingEscape());
  EXPECT_FALSE(parser.hasEvent());
}

TEST_F(InputParserTest, EscapeKeyFlush) {
  parser.feed(27);
  EXPECT_TRUE(parser.pendingEscape());
  parser.flushEscape();
  EXPECT_FALSE(parser.pendingEscape());
  EXPECT_TRUE(parser.hasEvent());
  EXPECT_EQ(getKey(parser.nextEvent()).key, 27);
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

  EXPECT_EQ(getKey(parser.nextEvent()).key, '1');
  EXPECT_EQ(getKey(parser.nextEvent()).key, '2');
  EXPECT_EQ(getKey(parser.nextEvent()).key, '3');
}

TEST_F(InputParserTest, SpecialCharacterKeys) {
  parser.feed(' ');
  parser.feed('\t');

  EXPECT_EQ(getKey(parser.nextEvent()).key, ' ');
  EXPECT_EQ(getKey(parser.nextEvent()).key, '\t');
}

} // namespace vextr::utils
